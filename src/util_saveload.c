#include "mapedit_main.h"

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
       ((elements = fread(set->mapWidth,  2, 1, set->file)) != 1) ||
       ((elements = fread(set->mapHeight, 2, 1, set->file)) != 1) ||
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
