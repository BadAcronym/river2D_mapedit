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

    if(editor->filename.data)
    {
        free((void*)editor->filename.data);
    }

    StringView  ext = cstr_sv(".rte");
    const char* pos = sv_filename.data + sv_filename.size - 4;

    if(sv_find(ext, sv_filename) != pos)
    {
        const char *appended = sv_concat(sv_filename, ext);
        editor->filename     = cstr_sv(appended);
    }
    else
    {
        editor->filename = sv_filename;
    }

    const char *cstr_filename = sv_cstr(editor->filename);
    FILE *file = fopen(cstr_filename, "wb");
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
        free((void*)cstr_filename);
        return;
    }
    if((elements = fwrite(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        free((void*)cstr_filename);
        return;
    }
    if((elements = fwrite(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        free((void*)cstr_filename);
        return;
    }
    if((elements = fwrite(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "\033[0m\n");
        free((void*)cstr_filename);
        return;
    }

    river2D_syncImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], false);

    imgsurf_write_ptr(file, engine->planes[MAPEDIT_PLANE_TILESHEET].data,
                      IMGSURF_FILE_QOI,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].height,
                      IMGSURF_CHANNELS_BGRA, 8);

    uint64_t dataC = engine->planes[MAPEDIT_PLANE_TILESHEET].width  / editor->tilesize *
                     engine->planes[MAPEDIT_PLANE_TILESHEET].height / editor->tilesize;
    fwrite(editor->tileData, sizeof(TileMetadata), dataC, file);

    uint64_t indexC = editor->layers * editor->mapHeight * editor->mapWidth;
    fwrite(editor->placedTiles, sizeof(TileIndex), indexC, file);

    editor->lastSaveTime = river2D_queryTime();

    free((void*)cstr_filename);

    fprintf(stdout, "\nProject saved successfully.\n");
    fclose(file);
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

    if(editor->filename.data)
    {
        free((void*)editor->filename.data);
    }

    StringView  ext = cstr_sv(".rte");
    const char* pos = sv_filename.data + sv_filename.size - 4;

    if(sv_find(ext, sv_filename) != pos)
    {
        const char *appended = sv_concat(sv_filename, ext);
        editor->filename     = cstr_sv(appended);
    }
    else
    {
        editor->filename = sv_filename;
    }

#ifdef DEBUG
    fprintf(stderr, "\nloading file: "PRI_SV"\n", ARG_SV(editor->filename));
#endif

    const char *cstr_filename = sv_cstr(editor->filename);
    FILE *file = fopen(cstr_filename, "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file "
                "named \""PRI_SV"\".\033[0m\n", ARG_SV(editor->inputBuffer));
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        free((void*)cstr_filename);
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
            free((void*)cstr_filename);
            return;
        }
    }

    size_t elements = 0;
    if((elements = fread(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        free((void*)cstr_filename);
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        free((void*)cstr_filename);
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        free((void*)cstr_filename);
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", sv_cstr(editor->filename));
        free((void*)cstr_filename);
        return;
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
        free((void*)cstr_filename);
        return;
    }
    engine->planes[MAPEDIT_PLANE_TILESHEET].path = cstr_sv("loadProject");

    uint64_t maxdatabyte = engine->planes[MAPEDIT_PLANE_TILESHEET].width  *
                           engine->planes[MAPEDIT_PLANE_TILESHEET].height /
                           (editor->tilesize * editor->tilesize) * sizeof(TileMetadata);
    for(uint64_t i = 0; i < maxdatabyte && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->tileData)[i] = (uint8_t)byte;
    }

    uint64_t maxtilebyte = editor->layers * editor->mapWidth * editor->mapHeight *
                           sizeof(TileIndex);
    for(uint64_t i = 0; i < maxtilebyte && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->placedTiles)[i] = (uint8_t)byte;
    }

    free((void*)cstr_filename);

    fclose(file);
}
