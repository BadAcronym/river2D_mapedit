#include "river2D_main.h"
#include "imgsurf_main.h"

TileMap mapedit_loadTilemap
(
    EngineData        *engine,
    rvLoadMapSettings *set
){
    const char header[9] = "r2Dtiles";
    int byte;

    for(uint8_t i = 0; i < 8 && ((byte = fgetc(set->file)) != EOF); ++i)
    {
        if(byte != header[i])
        {
            set->errorcode = RV_ERROR_INVALID_HEADER;
            return (TileMap){0};
        }
    }

    size_t elements = 0;

    if(((elements = fread(set->tilesize,  2, 1, set->file)) != 1) ||
       ((elements = fread(set->mapWidth,  4, 1, set->file)) != 1) ||
       ((elements = fread(set->mapHeight, 4, 1, set->file)) != 1) ||
       ((elements = fread(set->mapLayers, 1, 1, set->file)) != 1)
    ){
        set->errorcode = RV_ERROR_INVALID_HEADER;
        return (TileMap){0};
    }

    if(set->tilesheet->data)
    {
        rvDestroyImage(set->tilesheet);
    }
    rvLoadImage_ptr(engine, set->file, set->tilesheet, RV_CHANNELS_BGRA, 8);

    if(!set->tilesheet->data)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to load tilesheet into ptr."
                "\033[0m\n");
        set->errorcode = RV_ERROR_LOADIMAGE_PTR;
        return (TileMap){0};
    }
    set->tilesheet->path = cstr_sv("mapedit_loadTilemap");

    uint64_t sheetW = set->tilesheet->width  / *set->tilesize;
    uint64_t sheetH = set->tilesheet->height / *set->tilesize;

    uint64_t maxdatabyte  = sheetW * sheetH * sizeof(TileMetadata);
    uint64_t maxindexbyte = *set->mapLayers * *set->mapWidth * *set->mapHeight *
                           sizeof(TileIndex);

    TileMap map = {0};

    map.metadata = malloc(maxdatabyte);
    map.indices  = malloc(maxindexbyte);

    for(uint64_t i = 0; i < maxdatabyte && ((byte = fgetc(set->file)) != EOF); ++i)
    {
        ((uint8_t*)map.metadata)[i] = (uint8_t)byte;
    }

    for(uint64_t i = 0; i < maxindexbyte && ((byte = fgetc(set->file)) != EOF); ++i)
    {
        ((uint8_t*)map.indices)[i] = (uint8_t)byte;
    }

    return map;
}

void mapedit_saveTilemap
(
    EngineData        *engine,
    rvSaveMapSettings *set
){
    const char header[9] = "r2Dtiles";
    fwrite(header, sizeof(header) - 1, 1, set->file);

    size_t elems = 0;

    if(((elems = fwrite(&set->tilesize,  2, 1, set->file)) != 1) ||
       ((elems = fwrite(&set->mapWidth,  4, 1, set->file)) != 1) ||
       ((elems = fwrite(&set->mapHeight, 4, 1, set->file)) != 1) ||
       ((elems = fwrite(&set->mapLayers, 1, 1, set->file)) != 1)
    ){
        set->errorcode = RV_ERROR_INVALID_HEADER;
        return;
    }

    rvSyncImage(engine, set->tilesheet, false);

    imgsurf_write_ptr(set->file, set->tilesheet->data, IMGSURF_FILE_QOI,
                      set->tilesheet->width, set->tilesheet->height,
                      IMGSURF_CHANNELS_BGRA, 8);

    if(!set->metadata)
    {
        set->errorcode = RV_ERROR_INVALID_METADATA;
        return;
    }
    else if(!set->indices)
    {
        set->errorcode = RV_ERROR_INVALID_INDICES;
        return;
    }

    uint64_t dataC = set->tilesheet->width  / set->tilesize *
                     set->tilesheet->height / set->tilesize;
    if((elems = fwrite(set->metadata, sizeof(TileMetadata), dataC, set->file) != dataC))
    {
        set->errorcode = RV_ERROR_WRITE_METADATA;
        return;
    }

    uint64_t indexC = set->mapLayers * set->mapHeight * set->mapWidth;
    if((elems = fwrite(set->indices, sizeof(TileIndex), indexC, set->file) != indexC))
    {
        set->errorcode = RV_ERROR_WRITE_INDICES;
        return;
    }
}
