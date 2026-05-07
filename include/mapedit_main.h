#pragma once

#include "river2D_main.h"
#include "string_view.h"

#define MAPEDIT_MAX_ACTIONS           0xFFFF

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
#define MAPEDIT_PLANE_ICON_SAVED      10
#define MAPEDIT_PLANE_CURSOR_DEFAULT  11
#define MAPEDIT_PLANE_CURSOR_HOVER    12
#define MAPEDIT_PLANE_CURSOR_PLACE    13
#define MAPEDIT_PLANE_CURSOR_NULL     14
#define MAPEDIT_PLANE_TILESHEET       15
#define MAPEDIT_PLANE_CURRENTLAYER    16
#define MAPEDIT_PLANE_CURRENTFILE     17

#define MAPEDIT_BUTTON_LEFTM          0
#define MAPEDIT_BUTTON_RIGHTM         1
#define MAPEDIT_BUTTON_MIDDLEM        2

#define MAPEDIT_KEY_LSHIFT            0
#define MAPEDIT_KEY_RSHIFT            1
#define MAPEDIT_KEY_LCTRL             2
#define MAPEDIT_KEY_RCTRL             3
#define MAPEDIT_KEY_MENU              4
#define MAPEDIT_KEY_ENTER             5
#define MAPEDIT_KEY_HOTBAR            6
#define MAPEDIT_KEY_BACKSPACE         7
#define MAPEDIT_KEY_SPACE             8
#define MAPEDIT_KEY_DELETE            9
#define MAPEDIT_KEY_LAYER0            10
#define MAPEDIT_KEY_LAYER1            11
#define MAPEDIT_KEY_LAYER2            12
#define MAPEDIT_KEY_LAYER3            13
#define MAPEDIT_KEY_LAYER4            14
#define MAPEDIT_KEY_LAYER5            15
#define MAPEDIT_KEY_LAYER6            16
#define MAPEDIT_KEY_LAYER7            17
#define MAPEDIT_KEY_LAYER8            18
#define MAPEDIT_KEY_LAYER9            19
#define MAPEDIT_KEY_REDO              20
#define MAPEDIT_KEY_UNDO              21

#define MAPEDIT_BIT_LEFTM             0x000000000001LLU
#define MAPEDIT_BIT_RIGHTM            0x000000000002LLU
#define MAPEDIT_BIT_MIDDLEM           0x000000000004LLU

#define MAPEDIT_BIT_ESCAPE            0x0000000000000001LLU
#define MAPEDIT_BIT_ENTER             0x0000000000000002LLU
#define MAPEDIT_BIT_TAB               0x0000000000000004LLU
#define MAPEDIT_BIT_LSHIFT            0x0000000000000008LLU
#define MAPEDIT_BIT_RSHIFT            0x0000000000000010LLU
#define MAPEDIT_BIT_LCTRL             0x0000000000000020LLU
#define MAPEDIT_BIT_RCTRL             0x0000000000000040LLU
#define MAPEDIT_BIT_BACKSPACE         0x0000000000000080LLU
#define MAPEDIT_BIT_SPACE             0x0000000000000100LLU
#define MAPEDIT_BIT_0                 0x0000000000000200LLU
#define MAPEDIT_BIT_1                 0x0000000000000400LLU
#define MAPEDIT_BIT_2                 0x0000000000000800LLU
#define MAPEDIT_BIT_3                 0x0000000000001000LLU
#define MAPEDIT_BIT_4                 0x0000000000002000LLU
#define MAPEDIT_BIT_5                 0x0000000000004000LLU
#define MAPEDIT_BIT_6                 0x0000000000008000LLU
#define MAPEDIT_BIT_7                 0x0000000000010000LLU
#define MAPEDIT_BIT_8                 0x0000000000020000LLU
#define MAPEDIT_BIT_9                 0x0000000000040000LLU
#define MAPEDIT_BIT_A                 0x0000000000080000LLU
#define MAPEDIT_BIT_B                 0x0000000000100000LLU
#define MAPEDIT_BIT_C                 0x0000000000200000LLU
#define MAPEDIT_BIT_D                 0x0000000000400000LLU
#define MAPEDIT_BIT_E                 0x0000000000800000LLU
#define MAPEDIT_BIT_F                 0x0000000001000000LLU
#define MAPEDIT_BIT_G                 0x0000000002000000LLU
#define MAPEDIT_BIT_H                 0x0000000004000000LLU
#define MAPEDIT_BIT_I                 0x0000000008000000LLU
#define MAPEDIT_BIT_J                 0x0000000010000000LLU
#define MAPEDIT_BIT_K                 0x0000000020000000LLU
#define MAPEDIT_BIT_L                 0x0000000040000000LLU
#define MAPEDIT_BIT_M                 0x0000000080000000LLU
#define MAPEDIT_BIT_N                 0x0000000100000000LLU
#define MAPEDIT_BIT_O                 0x0000000200000000LLU
#define MAPEDIT_BIT_P                 0x0000000400000000LLU
#define MAPEDIT_BIT_Q                 0x0000000800000000LLU
#define MAPEDIT_BIT_R                 0x0000001000000000LLU
#define MAPEDIT_BIT_S                 0x0000002000000000LLU
#define MAPEDIT_BIT_T                 0x0000004000000000LLU
#define MAPEDIT_BIT_U                 0x0000008000000000LLU
#define MAPEDIT_BIT_V                 0x0000010000000000LLU
#define MAPEDIT_BIT_W                 0x0000020000000000LLU
#define MAPEDIT_BIT_X                 0x0000040000000000LLU
#define MAPEDIT_BIT_Y                 0x0000080000000000LLU
#define MAPEDIT_BIT_Z                 0x0000100000000000LLU

#define MAPEDIT_STATE_NULL            0
#define MAPEDIT_STATE_MENU            1
#define MAPEDIT_STATE_EDIT            2
#define MAPEDIT_STATE_LOAD            3

#define MAPEDIT_FLAG_BIT_TILEPICKER   0x01UL

#define MAPEDIT_BIT_INVALID   0x01
#define MAPEDIT_BIT_ANIMATED  0x02
#define MAPEDIT_BIT_COLLISION 0x04

typedef struct Tile
{
    uint16_t x;
    uint16_t y;
    uint16_t frames;
    uint8_t  flags;
    uint8_t  fps;
}
Tile;

typedef struct Action
{
    River2D_Time stroke_start;
    uint64_t     map_index;
    Tile         prev_tile;
    Tile         new_tile;
}
Action;

typedef struct EditorData
{
    River2D_Time lastPresentTime;
    River2D_Time lastSaveTime;
    River2D_Time lastActionStart;
    uint8_t      currentState;
    uint8_t      previousState;
    uint16_t     tilesize;
    uint8_t      currentLayer;
    bool         isolate;
    uint8_t      layers;
    uint8_t      selectedX;
    uint8_t      selectedY;
    uint8_t      selectMult;
    uint32_t     mapWidth;
    uint32_t     mapHeight;
    uint64_t     editorflags;
    Button       new_b;
    Button       load_b;
    Button       save_b;
    Button       quit_b;
    Button       close_b;
    Tile         *tiles;
    Action       *actions;
    uint32_t     currentAction;
    String       projectName;
    String       filename;
    uint8_t      viewScroll;
    uint8_t      cursor;
    bool         confirmed;
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

extern void mapedit_scroll
(
    EditorData *editor,
    bool       increase
);

extern void mapedit_updateSelectSize
(
    EditorData *editor,
    bool       increase
);

extern void mapedit_processButtons
(
    EditorData         *editor,
    River2D_ControlMap *controls,
    uint32_t           button,
    bool               isDown
);

extern void mapedit_processKeys
(
    River2D_ControlMap *controls,
    uint8_t            key,
    bool               isDown
);

extern void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
);

extern void mapedit_placeSelectedTiles
(
    EditorData *editor,
    uint16_t   tileX,
    uint16_t   tileY
);

extern void mapedit_changeState
(
    EditorData *editor,
    uint8_t    nextState
);

extern void mapedit_saveProject
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_loadProject
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_undo
(
    EditorData *editor
);

extern void mapedit_redo
(
    EditorData *editor
);

extern void mapedit_drawMainMenu
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_pollMainMenu
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_drawEditor
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_pollEditor
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_drawFilePicker
(
    EngineData *engine
);

extern void mapedit_pollFilePicker
(
    EngineData *engine,
    EditorData *editor
);
