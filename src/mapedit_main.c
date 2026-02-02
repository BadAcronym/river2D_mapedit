#include "mapedit_main.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

void mapedit_init
(
    EngineData *engine,
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
){
    river2D_loadImage("assets/black.qoi", &engine->planes[MAPEDIT_PLANE_BG0], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_BG0].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }

    river2D_loadImage("assets/font_default_16.qoi", &engine->planes[MAPEDIT_PLANE_FONT0], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_FONT0].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load font image!\033[0m\n");
    }

    uint32_t charsize     = 16;
    uint32_t total_width  = 13 * charsize;
    uint32_t total_height = 6  * (charsize + 10);

    engine->planes[MAPEDIT_PLANE_HUD0].width  = total_width;
    engine->planes[MAPEDIT_PLANE_HUD0].height = total_height;
    engine->planes[MAPEDIT_PLANE_HUD0].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_HUD0], " MAP EDITOR",  MAPEDIT_PLANE_FONT0, 16, 1, 0, 0);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_HUD0], " NEW PROJECT", MAPEDIT_PLANE_FONT0, 16, 1, 0, 50);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_HUD0], "LOAD PROJECT", MAPEDIT_PLANE_FONT0, 16, 1, 0, 74);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_HUD0], "    QUIT", MAPEDIT_PLANE_FONT0, 16, 1, 0, 126);

    //transfer to mapedit_loadConfig at some point, when keybinds should be remappable
    engine->controls.keycodes[MAPEDIT_KEY_LEFTM]   = Button1;
    engine->controls.keycodes[MAPEDIT_KEY_MIDDLEM] = Button2;
    engine->controls.keycodes[MAPEDIT_KEY_RIGHTM]  = Button3;
}

void mapedit_update
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BG0], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_BG0].width,
                           engine->planes[MAPEDIT_PLANE_BG0].height);

    //later moved to some conditional (if main menu or something.)
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HUD0], RIVER2D_PICTOP_OVER,
                           engine->backbuffer.width  / 2 - engine->planes[MAPEDIT_PLANE_HUD0].width  / 2,
                           engine->backbuffer.height / 2 - engine->planes[MAPEDIT_PLANE_HUD0].height / 2,
                           0, 0,
                           engine->planes[MAPEDIT_PLANE_HUD0].width,
                           engine->planes[MAPEDIT_PLANE_HUD0].height);

    if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
    {

    }
}

// TODO: (mapedit #2) handle buttons!
// TODO: (mapedit #3) highlight buttons when hovered

void mapedit_processKeys
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
){
}

void mapedit_processButtons
(
    bool               isDown,
    uint64_t           button,
    River2D_ControlMap *controls
){
    if(button == controls->keycodes[MAPEDIT_KEY_LEFTM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LEFTM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(button == controls->keycodes[MAPEDIT_KEY_RIGHTM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_RIGHTM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_RIGHTM;
        }
    }
    else if(button == controls->keycodes[MAPEDIT_KEY_MIDDLEM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_MIDDLEM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_MIDDLEM;
        }
    }
}

void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
){
    Dimensions dim = river2D_getWindowSize(engine);
    engine->controls.pointer.x = (double)x / dim.width;
    engine->controls.pointer.y = (double)y / dim.height;
}
