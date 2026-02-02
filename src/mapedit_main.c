#include "mapedit_main.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

void mapedit_init
(
    EngineData *engine
){
    river2D_loadImage("assets/black.qoi", &engine->planes[0],
                      RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[0].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }
}

void mapedit_update
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine, River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,  uint32_t cropHeight),
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
){
    river2D_compositeImage(engine, &engine->planes[0], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[0].width,
                           engine->planes[0].height);
}

void mapedit_processControls
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
){
    return;
}
