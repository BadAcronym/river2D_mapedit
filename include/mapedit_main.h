#pragma once

#include "river2D_main.h"

extern void mapedit_init(void);

extern void mapedit_update
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine, River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,  uint32_t cropHeight),
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
);

extern void mapedit_processControls
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
);
