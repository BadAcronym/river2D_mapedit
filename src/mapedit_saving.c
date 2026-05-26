#include "mapedit_main.h"
#include "imgsurf_main.h"
#include "util_saveload.h"

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

    rvLoadMapSettings set;
    set.tilesheet = &engine->planes[MAPEDIT_PLANE_TILESHEET];
    set.tilesize  = &editor->tilesize;
    set.mapWidth  = &editor->mapWidth;
    set.mapHeight = &editor->mapHeight;
    set.layers    = &editor->layers;
    set.file      = file;

    TileMap map = mapedit_loadTilemap(engine, &set);

    if(editor->tileData)
    {
        free(editor->tileData);
    }
    editor->tileData = map.metadata;

    if(editor->placedTiles)
    {
        free(editor->placedTiles);
    }
    editor->placedTiles = map.indices;

    if(set.errorcode == RV_ERROR_INVALID_HEADER)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", cstr_filename);
    }

    free((void*)cstr_filename);
    fclose(file);
}

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
