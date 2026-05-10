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

    StringView sv_filename = {0};
    sv_filename.data       = editor->inputBuffer.data;

    for(size_t i = 0; editor->inputBuffer.size; ++i)
    {
        if(editor->inputBuffer.data[i] == '\0')
        {
            break;
        }

        ++sv_filename.size;
    }

    StringView  ext = cstr_sv(".rte");
    const char* pos = sv_filename.data + sv_filename.size - 4;

    if(sv_find(ext, sv_filename) != pos)
    {
        editor->filename = cstr_sv(sv_concat(sv_filename, ext));
    }
    else
    {
        editor->filename = sv_filename;
    }

    FILE *file = fopen(sv_cstr(editor->filename), "wb");
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

    river2D_syncImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], false);

    imgsurf_write_ptr(file, engine->planes[MAPEDIT_PLANE_TILESHEET].data,
                      IMGSURF_FILE_QOI,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].height,
                      IMGSURF_CHANNELS_BGRA, 8);

    fclose(file);

    fprintf(stdout, "\nProject saved successfully.\n");

    editor->lastSaveTime = river2D_queryTime();
}

void mapedit_loadProject
(
    EngineData *engine,
    EditorData *editor
){
    StringView sv_filename = {0};
    sv_filename.data       = editor->inputBuffer.data;

    for(size_t i = 0; editor->inputBuffer.size; ++i)
    {
        if(editor->inputBuffer.data[i] == '\0')
        {
            break;
        }

        ++sv_filename.size;
    }

    StringView  ext = cstr_sv(".rte");
    const char* pos = sv_filename.data + sv_filename.size - 4;

    if(sv_find(ext, sv_filename) != pos)
    {
        editor->filename = cstr_sv(sv_concat(sv_filename, ext));
    }
    else
    {
        editor->filename = sv_filename;
    }

#ifdef DEBUG
    fprintf(stderr, "\nloading file: "PRI_SV"\n", ARG_SV(editor->filename));
#endif

    FILE *file = fopen(sv_cstr(editor->filename), "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file "
                "named \""PRI_SV"\".\033[0m\n", ARG_SV(editor->inputBuffer));
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
                    PRI_SV"\033[0m\n", ARG_SV(editor->inputBuffer));
            return;
        }
    }

    size_t elements = 0;
    if((elements = fread(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
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

    river2D_loadImage_ptr(engine, file, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                          RIVER2D_CHANNELS_BGRA, 8);

    if(!engine->planes[MAPEDIT_PLANE_TILESHEET].data)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to load tilesheet into ptr."
                "\033[0m\n");
        return;
    }

    engine->planes[MAPEDIT_PLANE_TILESHEET].path = cstr_sv("loadProject");

    free((void*)sv_cstr(editor->filename));

    fclose(file);
}
