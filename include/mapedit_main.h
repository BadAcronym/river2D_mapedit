#pragma once

#include "river2D_main.h"

#define MAPEDIT_PLANE_BG0   0
#define MAPEDIT_PLANE_FONT0 1
#define MAPEDIT_PLANE_HUD0  2
#define MAPEDIT_PLANE_HUD1  3
#define MAPEDIT_PLANE_HUD2  4
#define MAPEDIT_PLANE_HUD3  5

extern void mapedit_init
(
    EngineData *engine,
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
);

extern void mapedit_update
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine, River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,  uint32_t cropHeight)
);

extern void mapedit_processControls
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
);
