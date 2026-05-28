#include "mapedit_main.h"
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
       ((elements = fread(set->layers,    1, 1, set->file)) != 1)
    ){
        set->errorcode = RV_ERROR_INVALID_HEADER;
        return (TileMap){0};
    }

    if(set->tilesheet->data)
    {
        river2D_destroyImage(set->tilesheet);
    }
    river2D_loadImage_ptr(engine, set->file, set->tilesheet, RIVER2D_CHANNELS_BGRA, 8);

    if(!set->tilesheet->data)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to load tilesheet into ptr."
                "\033[0m\n");
        set->errorcode = RV_ERROR_LOADIMAGE_PTR;
        return (TileMap){0};
    }
    set->tilesheet->path = cstr_sv("mapedit_readRTE");

    uint64_t maxdatabyte = engine->planes[MAPEDIT_PLANE_TILESHEET].width  *
                           engine->planes[MAPEDIT_PLANE_TILESHEET].height /
                           (*set->tilesize * *set->tilesize) * sizeof(TileMetadata);

    uint64_t maxindexbyte = *set->layers * *set->mapWidth * *set->mapHeight *
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

    size_t elements = 0;

    if(((elements = fwrite(&set->tilesize,  2, 1, set->file)) != 1) ||
       ((elements = fwrite(&set->mapWidth,  4, 1, set->file)) != 1) ||
       ((elements = fwrite(&set->mapHeight, 4, 1, set->file)) != 1) ||
       ((elements = fwrite(&set->layers,    1, 1, set->file)) != 1)
    ){
        set->errorcode = RV_ERROR_INVALID_HEADER;
        return;
    }

    river2D_syncImage(engine, set->tilesheet, false);

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
    fwrite(set->metadata, sizeof(TileMetadata), dataC, set->file);

    uint64_t indexC = set->layers * set->mapHeight * set->mapWidth;
    fwrite(set->indices, sizeof(TileIndex), indexC, set->file);
}
