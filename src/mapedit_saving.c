#include "mapedit_main.h"
#include "imgsurf_main.h"

void mapedit_saveProject
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src                 = &engine->planes[MAPEDIT_PLANE_ICON_SAVING];
    comp.dst                 = &engine->backbuffer;
    comp.pictop              = RIVER2D_PICTOP_OVER;
    comp.cropWidth           = engine->planes[MAPEDIT_PLANE_ICON_SAVING].width;
    comp.cropHeight          = engine->planes[MAPEDIT_PLANE_ICON_SAVING].height;
    comp.offsetDstX          = 16;
    comp.offsetDstY          = 16;

    river2D_compositeImage(engine, &comp);

    engine->bltBuffer(engine);

    StringView sv_filename = str_sv_cpy(editor->filename);

    StringView  ext = cstr_sv(".rte");
    const char* pos = sv_filename.data + sv_filename.size - 4;

    if(sv_find(ext, sv_filename) != pos)
    {
        // TESTING: DEBUG
        fprintf(stderr, "pos: %p\n", pos);
        fprintf(stderr, "found: %p\n", sv_find(ext, sv_filename));
        fprintf(stderr, "attaching "PRI_SV" to "PRI_SV".\n",
                ARG_SV(ext), ARG_SV(sv_filename));
        StringView final = cstr_sv(sv_concat(sv_filename, ext));
        fprintf(stderr, "after: "PRI_SV"\n", ARG_SV(final));
    }

    // TESTING:
    const char *filename_cstr = "";

    FILE *file = fopen(filename_cstr, "wb");
    if(!file)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: could not open file for saving: "
                PRI_SV".\033[0m\n", ARG_SV(editor->filename));
        free((void*)sv_filename.data);
        return;
    }

    const char header[9] = "r2Dtiles";
    fwrite(header, sizeof(header) - 1, 1, file);

    size_t elements = 0;

    if((elements = fwrite(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "\033[0m\n");
        return;
    }

    uint64_t tilecount = editor->layers * editor->mapHeight * editor->mapWidth;
    fwrite(editor->tiles, sizeof(Tile), tilecount, file);

    imgsurf_write_ptr(file, engine->planes[MAPEDIT_PLANE_TILESHEET].data,
                      IMGSURF_FILE_QOI,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].height,
                      IMGSURF_CHANNELS_BGRA, 8);

    fclose(file);

    free((void*)filename_cstr);
    fprintf(stdout, "Project saved successfully.\n");

    editor->lastSaveTime = river2D_queryTime();
}

void mapedit_loadProject
(
    EngineData *engine,
    EditorData *editor
){
    StringView *name = (StringView*)&editor->filename;

    const char *filename_cstr;
    StringView ext = cstr_sv(".rte");
    char*      pos = editor->filename.data + editor->filename.size - 4;

    if(sv_find(ext, *((StringView*)&editor->filename)) != pos)
    {
        filename_cstr = sv_concat(*((StringView*)(&editor->filename)), ext);
    }
    else
    {
        filename_cstr = sv_cstr(*((StringView*)(&editor->filename)));
    }

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

    FILE *file = fopen(filename_cstr, "rb");
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
                "\033[0m\n", filename_cstr);
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename_cstr);
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename_cstr);
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", filename_cstr);
        return;
    }

    uint64_t maxtilebyte = editor->layers * editor->mapWidth * editor->mapHeight *
                           sizeof(Tile);

    for(uint64_t i = 0; i < maxtilebyte && ((byte = fgetc(file)) != EOF); ++i)
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

    free((void*)filename_cstr);

    fclose(file);
}
