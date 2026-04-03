#pragma once

#include "river2D_main.h"

#define MAPEDIT_PLANE_BACKGROUND      0
#define MAPEDIT_PLANE_VOID            1
#define MAPEDIT_PLANE_HIGHLIGHT       2
#define MAPEDIT_PLANE_FONT16          3
#define MAPEDIT_PLANE_MAINMENU        4
#define MAPEDIT_PLANE_PAUSEMENU       5
#define MAPEDIT_PLANE_SELECTTILE      6
#define MAPEDIT_PLANE_HUD2            7
#define MAPEDIT_PLANE_HUD3            8
#define MAPEDIT_PLANE_ICON_SAVING     9
#define MAPEDIT_PLANE_ICON_SAVED     10
#define MAPEDIT_PLANE_CURSOR_DEFAULT 11
#define MAPEDIT_PLANE_CURSOR_HOVER   12
#define MAPEDIT_PLANE_CURSOR_PLACE   13
#define MAPEDIT_PLANE_CURSOR_NULL    14
#define MAPEDIT_PLANE_TILESHEET      15
#define MAPEDIT_PLANE_CURRENTLAYER   16

#define MAPEDIT_BUTTON_LEFTM       0
#define MAPEDIT_BUTTON_RIGHTM      1
#define MAPEDIT_BUTTON_MIDDLEM     2

#define MAPEDIT_KEY_ESCAPE     0
#define MAPEDIT_KEY_SHIFT      1
#define MAPEDIT_KEY_QUIT       2
#define MAPEDIT_KEY_SAVE       3
#define MAPEDIT_KEY_TILEPICKER 4
#define MAPEDIT_KEY_INC_SIZE   5
#define MAPEDIT_KEY_RED_SIZE   6
#define MAPEDIT_KEY_LAYER0     7
#define MAPEDIT_KEY_LAYER1     8
#define MAPEDIT_KEY_LAYER2     9
#define MAPEDIT_KEY_LAYER3     10
#define MAPEDIT_KEY_LAYER4     11
#define MAPEDIT_KEY_LAYER5     12
#define MAPEDIT_KEY_LAYER6     13
#define MAPEDIT_KEY_LAYER7     14
#define MAPEDIT_KEY_LAYER8     15
#define MAPEDIT_KEY_LAYER9     16
#define MAPEDIT_KEY_ZOOMIN     17
#define MAPEDIT_KEY_ZOOMOUT    18
#define MAPEDIT_KEY_A          19
#define MAPEDIT_KEY_B          20
#define MAPEDIT_KEY_C          21
#define MAPEDIT_KEY_D          22
#define MAPEDIT_KEY_E          23
#define MAPEDIT_KEY_F          24
#define MAPEDIT_KEY_G          25
#define MAPEDIT_KEY_H          26
#define MAPEDIT_KEY_I          27
#define MAPEDIT_KEY_J          28
#define MAPEDIT_KEY_K          29
#define MAPEDIT_KEY_L          30
#define MAPEDIT_KEY_M          31
#define MAPEDIT_KEY_N          32
#define MAPEDIT_KEY_O          33
#define MAPEDIT_KEY_P          34
#define MAPEDIT_KEY_Q          35
#define MAPEDIT_KEY_R          36
#define MAPEDIT_KEY_S          37
#define MAPEDIT_KEY_T          38
#define MAPEDIT_KEY_U          39
#define MAPEDIT_KEY_V          40
#define MAPEDIT_KEY_W          41
#define MAPEDIT_KEY_X          42
#define MAPEDIT_KEY_Y          43
#define MAPEDIT_KEY_Z          44

#define MAPEDIT_BIT_LEFTM      0x000000000001
#define MAPEDIT_BIT_RIGHTM     0x000000000002
#define MAPEDIT_BIT_MIDDLEM    0x000000000004

#define MAPEDIT_BIT_ESCAPE     0x000000000001
#define MAPEDIT_BIT_SHIFT      0x000000000002
#define MAPEDIT_BIT_SAVE       0x000000000004
#define MAPEDIT_BIT_QUIT       0x000000000008
#define MAPEDIT_BIT_TILEPICKER 0x000000000010
#define MAPEDIT_BIT_INC_SIZE   0x000000000020
#define MAPEDIT_BIT_RED_SIZE   0x000000000040
#define MAPEDIT_BIT_LAYER0     0x000000000080
#define MAPEDIT_BIT_LAYER1     0x000000000100
#define MAPEDIT_BIT_LAYER2     0x000000000200
#define MAPEDIT_BIT_LAYER3     0x000000000400
#define MAPEDIT_BIT_LAYER4     0x000000000800
#define MAPEDIT_BIT_LAYER5     0x000000001000
#define MAPEDIT_BIT_LAYER6     0x000000002000
#define MAPEDIT_BIT_LAYER7     0x000000004000
#define MAPEDIT_BIT_LAYER8     0x000000008000
#define MAPEDIT_BIT_LAYER9     0x000000010000
#define MAPEDIT_BIT_ZOOMIN     0x000000020000
#define MAPEDIT_BIT_ZOOMOUT    0x000000040000
#define MAPEDIT_BIT_A          0x000000080000
#define MAPEDIT_BIT_B          0x000000100000
#define MAPEDIT_BIT_C          0x000000200000
#define MAPEDIT_BIT_D          0x000000400000
#define MAPEDIT_BIT_E          0x000000800000
#define MAPEDIT_BIT_F          0x000001000000
#define MAPEDIT_BIT_G          0x000002000000
#define MAPEDIT_BIT_H          0x000004000000
#define MAPEDIT_BIT_I          0x000008000000
#define MAPEDIT_BIT_J          0x000010000000
#define MAPEDIT_BIT_K          0x000020000000
#define MAPEDIT_BIT_L          0x000040000000
#define MAPEDIT_BIT_M          0x000080000000
#define MAPEDIT_BIT_N          0x000100000000
#define MAPEDIT_BIT_O          0x000200000000
#define MAPEDIT_BIT_P          0x000400000000
#define MAPEDIT_BIT_Q          0x000800000000
#define MAPEDIT_BIT_R          0x001000000000
#define MAPEDIT_BIT_S          0x002000000000
#define MAPEDIT_BIT_T          0x004000000000
#define MAPEDIT_BIT_U          0x008000000000
#define MAPEDIT_BIT_V          0x010000000000
#define MAPEDIT_BIT_W          0x020000000000
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
    River2D_Time lastPresentTime;
    River2D_Time lastSaveTime;
    uint8_t      current_state;
    uint8_t      previous_state;
    uint16_t     tilesize;
    uint8_t      currentLayer;
    uint8_t      layers;
    uint8_t      selectedX;
    uint8_t      selectedY;
    uint8_t      selectMult;
    uint32_t     map_width;
    uint32_t     map_height;
    uint64_t     editorflags;
    Rect         button_new;
    Rect         button_load;
    Rect         button_save;
    Rect         button_quit;
    Rect         button_tilepicker_close;
    Tile         *tiles;
    const char   *projectName;
}
EditorData;

extern void mapedit_init
(
    EngineData *engine,
    EditorData *editor
);

int32_t mapedit_shutdown
(
    EditorData *editor
);

extern void mapedit_update
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_updateSelectSize
(
    EditorData *editor,
    bool       increase
);

extern void mapedit_processButtons
(
    River2D_ControlMap *controls,
    uint64_t           button,
    bool               isDown
);

extern void mapedit_processKeys
(
    River2D_ControlMap *controls,
    uint64_t           key,
    bool               isDown
);

extern void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
);
