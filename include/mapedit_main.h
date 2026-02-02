#pragma once

#include "river2D_main.h"

#define MAPEDIT_PLANE_BACKGROUND     0
#define MAPEDIT_PLANE_VOID           1
#define MAPEDIT_PLANE_HIGHLIGHT      2
#define MAPEDIT_PLANE_FONT16         3
#define MAPEDIT_PLANE_MENU           4
#define MAPEDIT_PLANE_SELECTTILE     5
#define MAPEDIT_PLANE_CLOSE          6
#define MAPEDIT_PLANE_HUD3           7
#define MAPEDIT_PLANE_CURSOR_DEFAULT 8
#define MAPEDIT_PLANE_CURSOR_HOVER   9
#define MAPEDIT_PLANE_CURSOR_PLACE   10
#define MAPEDIT_PLANE_CURSOR_NULL    11
#define MAPEDIT_PLANE_TILESHEET      12

#define MAPEDIT_KEY_LEFTM      0
#define MAPEDIT_KEY_RIGHTM     1
#define MAPEDIT_KEY_MIDDLEM    2
#define MAPEDIT_KEY_ESCAPE     3
#define MAPEDIT_KEY_QUIT       4
#define MAPEDIT_KEY_TILEPICKER 5

#define MAPEDIT_BIT_LEFTM      1
#define MAPEDIT_BIT_RIGHTM     2
#define MAPEDIT_BIT_MIDDLEM    4
#define MAPEDIT_BIT_ESCAPE     8
#define MAPEDIT_BIT_QUIT       16
#define MAPEDIT_BIT_TILEPICKER 32

#define MAPEDIT_STATE_MENU  0
#define MAPEDIT_STATE_EDIT  1
#define MAPEDIT_STATE_LOAD  2

#define MAPEDIT_FLAG_BIT_TILEPICKER 1

typedef struct EditorData
{
    Rect     button_new;
    Rect     button_load;
    Rect     button_quit;
    Rect     button_tilepicker_close;
    uint8_t  state;
    uint64_t editorflags;
}
EditorData;

extern void mapedit_init
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
);

extern void mapedit_update
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine, River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,  uint32_t cropHeight)
);

extern void mapedit_processKeys
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
);

extern void mapedit_processButtons
(
    bool               isDown,
    uint64_t           button,
    River2D_ControlMap *controls
);

extern void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
);
