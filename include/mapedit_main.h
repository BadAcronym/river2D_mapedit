#pragma once

#include "river2D_main.h"
#include "string_view.h"

#define MAPEDIT_MAX_ACTIONS           0xFFFF

#define MAPEDIT_PLANE_BACKGROUND      0
#define MAPEDIT_PLANE_VOID            1
#define MAPEDIT_PLANE_COLLISION       2
#define MAPEDIT_PLANE_HIGHLIGHT       3
#define MAPEDIT_PLANE_HIGHLIGHT_SOLID 4
#define MAPEDIT_PLANE_FONT16          5
#define MAPEDIT_PLANE_MAINMENU        6
#define MAPEDIT_PLANE_PAUSEMENU       7
#define MAPEDIT_PLANE_TILEPICKER      8
#define MAPEDIT_PLANE_CONTEXTMENU     9
#define MAPEDIT_PLANE_HUD2            10
#define MAPEDIT_PLANE_HUD3            11
#define MAPEDIT_PLANE_ICON_SAVING     12
#define MAPEDIT_PLANE_ICON_SAVED      13
#define MAPEDIT_PLANE_CURSOR_DEFAULT  14
#define MAPEDIT_PLANE_CURSOR_HOVER    15
#define MAPEDIT_PLANE_CURSOR_PLACE    16
#define MAPEDIT_PLANE_CURSOR_NULL     17
#define MAPEDIT_PLANE_TILESHEET       18
#define MAPEDIT_PLANE_CURRENTLAYER    19
#define MAPEDIT_PLANE_CURRENTFILE     20
#define MAPEDIT_PLANE_FPS             21

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
#define MAPEDIT_KEY_DELETE            8
#define MAPEDIT_KEY_LAYER0            9
#define MAPEDIT_KEY_LAYER1            10
#define MAPEDIT_KEY_LAYER2            11
#define MAPEDIT_KEY_LAYER3            12
#define MAPEDIT_KEY_LAYER4            13
#define MAPEDIT_KEY_LAYER5            14
#define MAPEDIT_KEY_LAYER6            15
#define MAPEDIT_KEY_LAYER7            16
#define MAPEDIT_KEY_LAYER8            17
#define MAPEDIT_KEY_LAYER9            18
#define MAPEDIT_KEY_REDO              19
#define MAPEDIT_KEY_UNDO              20
#define MAPEDIT_KEY_DECREASE          21
#define MAPEDIT_KEY_INCREASE          22
#define MAPEDIT_KEY_SAVE              23
#define MAPEDIT_KEY_QUIT              24
#define MAPEDIT_KEY_TILEPICKER        25
#define MAPEDIT_KEY_WIREFRAME         26
#define MAPEDIT_KEY_UP                27
#define MAPEDIT_KEY_DOWN              28
#define MAPEDIT_KEY_LEFT              29
#define MAPEDIT_KEY_RIGHT             30

#define MAPEDIT_BIT_LEFTM             0x000000000001LLU
#define MAPEDIT_BIT_RIGHTM            0x000000000002LLU
#define MAPEDIT_BIT_MIDDLEM           0x000000000004LLU

#define MAPEDIT_BIT_LSHIFT            0x0000000000000001LLU
#define MAPEDIT_BIT_RSHIFT            0x0000000000000002LLU
#define MAPEDIT_BIT_LCTRL             0x0000000000000004LLU
#define MAPEDIT_BIT_RCTRL             0x0000000000000008LLU
#define MAPEDIT_BIT_MENU              0x0000000000000010LLU
#define MAPEDIT_BIT_ENTER             0x0000000000000020LLU
#define MAPEDIT_BIT_HOTBAR            0x0000000000000040LLU
#define MAPEDIT_BIT_BACKSPACE         0x0000000000000080LLU
#define MAPEDIT_BIT_DELETE            0x0000000000000100LLU
#define MAPEDIT_BIT_LAYER0            0x0000000000000200LLU
#define MAPEDIT_BIT_LAYER1            0x0000000000000400LLU
#define MAPEDIT_BIT_LAYER2            0x0000000000000800LLU
#define MAPEDIT_BIT_LAYER3            0x0000000000001000LLU
#define MAPEDIT_BIT_LAYER4            0x0000000000002000LLU
#define MAPEDIT_BIT_LAYER5            0x0000000000004000LLU
#define MAPEDIT_BIT_LAYER6            0x0000000000008000LLU
#define MAPEDIT_BIT_LAYER7            0x0000000000010000LLU
#define MAPEDIT_BIT_LAYER8            0x0000000000020000LLU
#define MAPEDIT_BIT_LAYER9            0x0000000000040000LLU
#define MAPEDIT_BIT_REDO              0x0000000000080000LLU
#define MAPEDIT_BIT_UNDO              0x0000000000100000LLU
#define MAPEDIT_BIT_DECREASE          0x0000000000200000LLU
#define MAPEDIT_BIT_INCREASE          0x0000000000400000LLU
#define MAPEDIT_BIT_SAVE              0x0000000000800000LLU
#define MAPEDIT_BIT_QUIT              0x0000000001000000LLU
#define MAPEDIT_BIT_TILEPICKER        0x0000000002000000LLU
#define MAPEDIT_BIT_WIREFRAME         0x0000000004000000LLU
#define MAPEDIT_BIT_UP                0x0000000008000000LLU
#define MAPEDIT_BIT_DOWN              0x0000000010000000LLU
#define MAPEDIT_BIT_LEFT              0x0000000020000000LLU
#define MAPEDIT_BIT_RIGHT             0x0000000040000000LLU

#define MAPEDIT_STATE_NULL            0
#define MAPEDIT_STATE_MENU            1
#define MAPEDIT_STATE_EDIT            2
#define MAPEDIT_STATE_LOAD            3
#define MAPEDIT_STATE_SAVEAS          4

#define MAPEDIT_FLAG_TILEPICKER  0x01
#define MAPEDIT_FLAG_WIREFRAME   0x02
#define MAPEDIT_FLAG_CONTEXTMENU 0x04

#define RV_TILE_BIT_ANIMATED  0x01
#define RV_TILE_BIT_COLLISION 0x02

#define MAPEDIT_CHOICE_SHOW_FPS_BIT 0x01

typedef struct Action
{
    River2D_Time stroke_start;
    uint64_t     map_index;
    TileIndex    prev_tile;
    TileIndex    new_tile;
}
Action;

typedef struct EditorData
{
    uint16_t     runningFrames;
    River2D_Time lastFPSTime;
    River2D_Time lastUpdateTime;
    River2D_Time lastPresentTime;
    River2D_Time lastSaveTime;
    River2D_Time lastActionStart;
    uint8_t      currentState;
    uint8_t      previousState;
    uint8_t      currentLayer;
    bool         isolate;
    uint16_t     selectedX;
    uint16_t     selectedY;
    uint8_t      selectMult;
    uint16_t     tilesize;
    uint32_t     mapWidth;
    uint32_t     mapHeight;
    uint8_t      mapLayers;
    TileMetadata *tileData;
    TileIndex    *placedTiles;
    uint8_t      flags;
    Button       new_b;
    Button       load_b;
    Button       save_b;
    Button       saveas_b;
    Button       quit_b;
    Button       close_b;
    Button       collision_b;
    Button       animation_b;
    uint8_t      i_button;
    Action       *actions;
    uint32_t     currentAction;
    String       inputBuffer;
    StringView   filename;
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

extern void mapedit_present
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
    AsciiKey           key,
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
    EngineData *engine,
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

extern void mapedit_pollLoadFile
(
    EngineData *engine,
    EditorData *editor
);

extern void mapedit_pollSaveFile
(
    EngineData *engine,
    EditorData *editor
);
