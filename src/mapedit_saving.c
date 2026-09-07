#include "mapedit_main.h"
#include "pd_print_macros.h"

void meLoadProject
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
    char appended[4096] = {0};

    if(sv_find(ext, sv_filename) != pos)
    {
        sv_concat(sv_filename, ext, appended);
        editor->filename = cstr_sv(appended);
    }
    else
    {
        editor->filename = sv_filename;
    }

    PD_DEBUG("loading file: " PRI_SV"\n", ARG_SV(editor->filename));

    char cstr_filename[4096] = {0};
    sv_cstr(editor->filename, cstr_filename);
    FILE *file = fopen(cstr_filename, "rb");
    if(!file)
    {
        PD_ERROR("could not open file named '"PRI_SV"'.", ARG_SV(editor->inputBuffer));
        meChangeState(editor, ME_STATE_MENU);
        return;
    }

    rvLoadMapSettings set = {0};
    set.file      = file;
    set.tilesize  = &editor->tilesize;
    set.mapWidth  = &editor->mapWidth;
    set.mapHeight = &editor->mapHeight;
    set.mapLayers = &editor->mapLayers;
    set.tilesheet = &engine->planes[ME_PLANE_TILESHEET];

    TileMap map = rvLoadTilemap(engine, &set);
    if(set.errorcode)
    {
        PD_ERROR("failed to load project file: '%s'. Code: %u",
                 cstr_filename, set.errorcode);
        fclose(file);
        return;
    }

    PD_DEBUG("mapWidth: %u",  editor->mapWidth);
    PD_DEBUG("mapHeight: %u", editor->mapHeight);
    PD_DEBUG("mapLayers: %u", editor->mapLayers);

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

    fclose(file);
}

void meSaveProject
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[ME_PLANE_ICON_SAVING];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.cropWidth  = engine->planes[ME_PLANE_ICON_SAVING].width;
    comp.cropHeight = engine->planes[ME_PLANE_ICON_SAVING].height;
    comp.offsetDstX = 16;
    comp.offsetDstY = 16;

    rvCompositeImage(engine, &comp);

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
    char appended[4096] = {0};

    if(sv_find(ext, sv_filename) != pos)
    {
        sv_concat(sv_filename, ext, appended);
        editor->filename = cstr_sv(appended);
    }
    else
    {
        editor->filename = sv_filename;
    }

    char cstr_filename[4096] = {0};
    sv_cstr(editor->filename, cstr_filename);
    FILE *file = fopen(cstr_filename, "wb");
    if(!file)
    {
        PD_ERROR("could not open file for saving: '"PRI_SV"'",
                 ARG_SV(editor->filename));
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
    set.tilesheet = &engine->planes[ME_PLANE_TILESHEET];

    rvSaveTilemap(engine, &set);
    if(set.errorcode)
    {
        PD_ERROR("failed to write savefile. Code: %u", set.errorcode);
        fclose(file);
        return;
    }

    editor->lastSaveTime = rvQueryTime();
    PD_DEBUG("project saved successfully.");
    fclose(file);
}
