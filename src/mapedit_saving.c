#include "mapedit_main.h"
#include "imgsurf_main.h"

void mapedit_loadProject
(
    EngineData *engine,
    EditorData *editor
){
    StringView *name = (StringView*)&editor->filename;
    const char *filename = sv_cstr(*name);

    if(!editor->filename.data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to find .rte file to load!"
                "\033[0m\n");
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        return;
    }

#ifdef DEBUG
    fprintf(stderr, "\nloading file: "PRI_SV"\n", ARG_SV(editor->filename));
#endif

    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file "
                "named \""PRI_SV"\".\033[0m\n", ARG_SV(editor->filename));
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        return;
    }

    const char header[9] = "r2Dtiles";
    int byte;

    for(uint8_t i = 0; i < 8 && ((byte = fgetc(file)) != EOF); ++i)
    {
        if(byte != header[i])
        {
            fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: "
                    PRI_SV"\033[0m\n", ARG_SV(editor->filename));
            return;
        }
    }

    size_t elements = 0;

    if((elements = fread(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename);
        return;
    }

    uint64_t maxtile = editor->layers * editor->mapWidth * editor->mapHeight *
                       sizeof(Tile);

    for(uint64_t i = 0; i < maxtile && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->tiles)[i] = (uint8_t)byte;
    }

    if(engine->planes[MAPEDIT_PLANE_TILESHEET].data)
    {
        river2D_destroyImage(&engine->planes[MAPEDIT_PLANE_TILESHEET]);
    }

    engine->planes[MAPEDIT_PLANE_TILESHEET].data = imgsurf_load_ptr(file,
            IMGSURF_FILE_QOI, &engine->planes[MAPEDIT_PLANE_TILESHEET].width,
            &engine->planes[MAPEDIT_PLANE_TILESHEET].height, IMGSURF_CHANNELS_BGRA, 8);

    engine->planes[MAPEDIT_PLANE_TILESHEET].path = cstr_sv("loadProject");

    fclose(file);
}
