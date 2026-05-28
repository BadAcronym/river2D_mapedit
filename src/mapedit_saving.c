#include "mapedit_main.h"
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

    rvLoadMapSettings set = {0};
    set.file      = file;
    set.tilesize  = &editor->tilesize;
    set.mapWidth  = &editor->mapWidth;
    set.mapHeight = &editor->mapHeight;
    set.mapLayers = &editor->mapLayers;
    set.tilesheet = &engine->planes[MAPEDIT_PLANE_TILESHEET];

    TileMap map = mapedit_loadTilemap(engine, &set);
    if(set.errorcode)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to load project file: %s. Code: %u"
                "\033[0m\n", cstr_filename, set.errorcode);
        free((void*)cstr_filename);
        fclose(file);
        return;
    }

    #ifdef DEBUG
    fprintf(stderr, "mapWidth: %u\n",  editor->mapWidth);
    fprintf(stderr, "mapHeight: %u\n", editor->mapHeight);
    fprintf(stderr, "mapLayers: %u\n", editor->mapLayers);
    #endif

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

    free((void*)cstr_filename);
    fclose(file);
}

void mapedit_saveProject
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[MAPEDIT_PLANE_ICON_SAVING];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RIVER2D_PICTOP_OVER;
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_ICON_SAVING].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_ICON_SAVING].height;
    comp.offsetDstX = 16;
    comp.offsetDstY = 16;

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

    // ASAN: use-after-free here, somehow.
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

    rvSaveMapSettings set = {0};
    set.file      = file;
    set.tilesize  = editor->tilesize;
    set.mapWidth  = editor->mapWidth;
    set.mapHeight = editor->mapHeight;
    set.mapLayers = editor->mapLayers;
    set.metadata  = editor->tileData;
    set.indices   = editor->placedTiles;
    set.tilesheet = &engine->planes[MAPEDIT_PLANE_TILESHEET];

    mapedit_saveTilemap(engine, &set);
    if(set.errorcode)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write savefile. Code: %u"
                "\033[0m\n", set.errorcode);
        return;
    }

    editor->lastSaveTime = river2D_queryTime();
    fprintf(stdout, "\nProject saved successfully.\n");

    free((void*)cstr_filename);
    fclose(file);
}
