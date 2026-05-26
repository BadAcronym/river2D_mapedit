#pragma once

#include "river2D_main.h"

extern TileMap mapedit_loadTilemap
(
    EngineData        *engine,
    rvLoadMapSettings *set
);

extern TileMap mapedit_saveTilemap
(
    EngineData        *engine,
    rvSaveMapSettings *set
);
