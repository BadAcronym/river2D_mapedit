#pragma once

#include "river2D_main.h"

#define MAPEDIT_PLANE_BACKGROUND     00
#define MAPEDIT_PLANE_VOID           01
#define MAPEDIT_PLANE_HIGHLIGHT      02
#define MAPEDIT_PLANE_FONT16         03
#define MAPEDIT_PLANE_MENU           04
#define MAPEDIT_PLANE_SELECTTILE     05
#define MAPEDIT_PLANE_HUD2           06
#define MAPEDIT_PLANE_HUD3           07
#define MAPEDIT_PLANE_CURSOR_DEFAULT 08
#define MAPEDIT_PLANE_CURSOR_HOVER   09
#define MAPEDIT_PLANE_CURSOR_PLACE   10
#define MAPEDIT_PLANE_CURSOR_NULL    11
#define MAPEDIT_PLANE_TILESHEET      12

#define MAPEDIT_KEY_LEFTM      00
#define MAPEDIT_KEY_RIGHTM     01
#define MAPEDIT_KEY_MIDDLEM    02
#define MAPEDIT_KEY_ESCAPE     03
#define MAPEDIT_KEY_SHIFT      04
#define MAPEDIT_KEY_QUIT       05
#define MAPEDIT_KEY_SAVE       06
#define MAPEDIT_KEY_TILEPICKER 07
#define MAPEDIT_KEY_LAYER0     08
#define MAPEDIT_KEY_LAYER1     09
#define MAPEDIT_KEY_LAYER2     10
#define MAPEDIT_KEY_LAYER3     11
#define MAPEDIT_KEY_LAYER4     12
#define MAPEDIT_KEY_LAYER5     13
#define MAPEDIT_KEY_LAYER6     14
#define MAPEDIT_KEY_LAYER7     15
#define MAPEDIT_KEY_LAYER8     16
#define MAPEDIT_KEY_LAYER9     17
#define MAPEDIT_KEY_ZOOMIN     18
#define MAPEDIT_KEY_ZOOMOUT    19
#define MAPEDIT_KEY_A          20
#define MAPEDIT_KEY_B          21
#define MAPEDIT_KEY_C          22
#define MAPEDIT_KEY_D          23
#define MAPEDIT_KEY_E          24
#define MAPEDIT_KEY_F          25
#define MAPEDIT_KEY_G          26
#define MAPEDIT_KEY_H          27
#define MAPEDIT_KEY_I          28
#define MAPEDIT_KEY_J          29
#define MAPEDIT_KEY_K          30
#define MAPEDIT_KEY_L          31
#define MAPEDIT_KEY_M          32
#define MAPEDIT_KEY_N          33
#define MAPEDIT_KEY_O          34
#define MAPEDIT_KEY_P          35
#define MAPEDIT_KEY_Q          36
#define MAPEDIT_KEY_R          37
#define MAPEDIT_KEY_S          38
#define MAPEDIT_KEY_T          39
#define MAPEDIT_KEY_U          40
#define MAPEDIT_KEY_V          41
#define MAPEDIT_KEY_X          42
#define MAPEDIT_KEY_Y          43
#define MAPEDIT_KEY_Z          44

#define MAPEDIT_BIT_LEFTM      0x000000000001
#define MAPEDIT_BIT_RIGHTM     0x000000000002
#define MAPEDIT_BIT_MIDDLEM    0x000000000004
#define MAPEDIT_BIT_ESCAPE     0x000000000008
#define MAPEDIT_BIT_SHIFT      0x000000000010
#define MAPEDIT_BIT_SAVE       0x000000000020
#define MAPEDIT_BIT_QUIT       0x000000000040
#define MAPEDIT_BIT_TILEPICKER 0x000000000080
#define MAPEDIT_BIT_LAYER0     0x000000000100
#define MAPEDIT_BIT_LAYER1     0x000000000200
#define MAPEDIT_BIT_LAYER2     0x000000000400
#define MAPEDIT_BIT_LAYER3     0x000000000800
#define MAPEDIT_BIT_LAYER4     0x000000001000
#define MAPEDIT_BIT_LAYER5     0x000000002000
#define MAPEDIT_BIT_LAYER6     0x000000004000
#define MAPEDIT_BIT_LAYER7     0x000000008000
#define MAPEDIT_BIT_LAYER8     0x000000010000
#define MAPEDIT_BIT_LAYER9     0x000000020000
#define MAPEDIT_BIT_ZOOMIN     0x000000040000
#define MAPEDIT_BIT_ZOOMOUT    0x000000080000
#define MAPEDIT_BIT_A          0x000000100000
#define MAPEDIT_BIT_B          0x000000200000
#define MAPEDIT_BIT_C          0x000000400000
#define MAPEDIT_BIT_D          0x000000800000
#define MAPEDIT_BIT_E          0x000001000000
#define MAPEDIT_BIT_F          0x000002000000
#define MAPEDIT_BIT_G          0x000004000000
#define MAPEDIT_BIT_H          0x000008000000
#define MAPEDIT_BIT_I          0x000010000000
#define MAPEDIT_BIT_J          0x000020000000
#define MAPEDIT_BIT_K          0x000040000000
#define MAPEDIT_BIT_L          0x000080000000
#define MAPEDIT_BIT_M          0x000100000000
#define MAPEDIT_BIT_N          0x000200000000
#define MAPEDIT_BIT_O          0x000400000000
#define MAPEDIT_BIT_P          0x000800000000
#define MAPEDIT_BIT_Q          0x001000000000
#define MAPEDIT_BIT_R          0x002000000000
#define MAPEDIT_BIT_S          0x004000000000
#define MAPEDIT_BIT_T          0x008000000000
#define MAPEDIT_BIT_U          0x010000000000
#define MAPEDIT_BIT_V          0x020000000000
#define MAPEDIT_BIT_X          0x040000000000
#define MAPEDIT_BIT_Y          0x080000000000
#define MAPEDIT_BIT_Z          0x100000000000

#define MAPEDIT_STATE_NULL 0
#define MAPEDIT_STATE_MENU 1
#define MAPEDIT_STATE_EDIT 2
#define MAPEDIT_STATE_LOAD 3

#define MAPEDIT_FLAG_BIT_TILEPICKER 0x01

typedef struct Tile
{
    uint16_t x;
    uint16_t y;
}
Tile;

typedef struct EditorData
{
    uint8_t    current_state;
    uint8_t    previous_state;
    uint8_t    tilesize;
    uint8_t    currentLayer;
    uint8_t    layers;
    uint8_t    selectedX;
    uint8_t    selectedY;
    uint32_t   map_width;
    uint32_t   map_height;
    uint64_t   editorflags;
    Rect       button_new;
    Rect       button_load;
    Rect       button_quit;
    Rect       button_tilepicker_close;
    Tile       *tiles;
    const char *projectName;
}
EditorData;

extern void mapedit_init
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetX, uint32_t offsetY)
);

int32_t mapedit_shutdown
(
    EditorData *editor
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
