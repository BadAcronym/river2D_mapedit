#pragma once

#include "river2D_main.h"

#include "pd_string_view.h"

#define ME_MAX_ACTIONS           0xFFFF

#define ME_PLANE_BACKGROUND      0
#define ME_PLANE_VOID            1
#define ME_PLANE_COLLISION       2
#define ME_PLANE_HIGHLIGHT       3
#define ME_PLANE_HIGHLIGHT_SOLID 4
#define ME_PLANE_FONT16          5
#define ME_PLANE_MAINMENU        6
#define ME_PLANE_PAUSEMENU       7
#define ME_PLANE_TILEPICKER      8
#define ME_PLANE_CONTEXTMENU     9
#define ME_PLANE_HUD2            10
#define ME_PLANE_HUD3            11
#define ME_PLANE_ICON_SAVING     12
#define ME_PLANE_ICON_SAVED      13
#define ME_PLANE_CURSOR_DEFAULT  14
#define ME_PLANE_CURSOR_HOVER    15
#define ME_PLANE_CURSOR_PLACE    16
#define ME_PLANE_CURSOR_NULL     17
#define ME_PLANE_TILESHEET       18
#define ME_PLANE_CURRENTLAYER    19
#define ME_PLANE_CURRENTFILE     20
#define ME_PLANE_FPS             21

#define ME_BUTTON_LEFTM          0
#define ME_BUTTON_RIGHTM         1
#define ME_BUTTON_MIDDLEM        2

#define ME_KEY_LSHIFT            0
#define ME_KEY_RSHIFT            1
#define ME_KEY_LCTRL             2
#define ME_KEY_RCTRL             3
#define ME_KEY_MENU              4
#define ME_KEY_ENTER             5
#define ME_KEY_HOTBAR            6
#define ME_KEY_BACKSPACE         7
#define ME_KEY_DELETE            8
#define ME_KEY_LAYER0            9
#define ME_KEY_LAYER1            10
#define ME_KEY_LAYER2            11
#define ME_KEY_LAYER3            12
#define ME_KEY_LAYER4            13
#define ME_KEY_LAYER5            14
#define ME_KEY_LAYER6            15
#define ME_KEY_LAYER7            16
#define ME_KEY_LAYER8            17
#define ME_KEY_LAYER9            18
#define ME_KEY_REDO              19
#define ME_KEY_UNDO              20
#define ME_KEY_DECREASE          21
#define ME_KEY_INCREASE          22
#define ME_KEY_SAVE              23
#define ME_KEY_QUIT              24
#define ME_KEY_TILEPICKER        25
#define ME_KEY_WIREFRAME         26
#define ME_KEY_UP                27
#define ME_KEY_DOWN              28
#define ME_KEY_LEFT              29
#define ME_KEY_RIGHT             30

#define ME_BIT_LEFTM             0x000000000001LLU
#define ME_BIT_RIGHTM            0x000000000002LLU
#define ME_BIT_MIDDLEM           0x000000000004LLU

#define ME_BIT_LSHIFT            0x0000000000000001LLU
#define ME_BIT_RSHIFT            0x0000000000000002LLU
#define ME_BIT_LCTRL             0x0000000000000004LLU
#define ME_BIT_RCTRL             0x0000000000000008LLU
#define ME_BIT_MENU              0x0000000000000010LLU
#define ME_BIT_ENTER             0x0000000000000020LLU
#define ME_BIT_HOTBAR            0x0000000000000040LLU
#define ME_BIT_BACKSPACE         0x0000000000000080LLU
#define ME_BIT_DELETE            0x0000000000000100LLU
#define ME_BIT_LAYER0            0x0000000000000200LLU
#define ME_BIT_LAYER1            0x0000000000000400LLU
#define ME_BIT_LAYER2            0x0000000000000800LLU
#define ME_BIT_LAYER3            0x0000000000001000LLU
#define ME_BIT_LAYER4            0x0000000000002000LLU
#define ME_BIT_LAYER5            0x0000000000004000LLU
#define ME_BIT_LAYER6            0x0000000000008000LLU
#define ME_BIT_LAYER7            0x0000000000010000LLU
#define ME_BIT_LAYER8            0x0000000000020000LLU
#define ME_BIT_LAYER9            0x0000000000040000LLU
#define ME_BIT_REDO              0x0000000000080000LLU
#define ME_BIT_UNDO              0x0000000000100000LLU
#define ME_BIT_DECREASE          0x0000000000200000LLU
#define ME_BIT_INCREASE          0x0000000000400000LLU
#define ME_BIT_SAVE              0x0000000000800000LLU
#define ME_BIT_QUIT              0x0000000001000000LLU
#define ME_BIT_TILEPICKER        0x0000000002000000LLU
#define ME_BIT_WIREFRAME         0x0000000004000000LLU
#define ME_BIT_UP                0x0000000008000000LLU
#define ME_BIT_DOWN              0x0000000010000000LLU
#define ME_BIT_LEFT              0x0000000020000000LLU
#define ME_BIT_RIGHT             0x0000000040000000LLU

#define ME_STATE_NULL            0
#define ME_STATE_MENU            1
#define ME_STATE_EDIT            2
#define ME_STATE_LOAD            3
#define ME_STATE_SAVEAS          4

#define ME_FLAG_TILEPICKER  0x01
#define ME_FLAG_WIREFRAME   0x02
#define ME_FLAG_CONTEXTMENU 0x04

#define ME_CHOICE_SHOW_FPS_BIT 0x01

typedef struct Action
{
    RiverTime stroke_start;
    u64       map_index;
    TileIndex prev_tile;
    TileIndex new_tile;
}
Action;

typedef struct EditorData
{
    u16          runningFrames;
    RiverTime    lastFPSTime;
    RiverTime    lastUpdateTime;
    RiverTime    lastPresentTime;
    RiverTime    lastSaveTime;
    RiverTime    lastActionStart;
    u08          currentState;
    u08          previousState;
    u08          currentLayer;
    bool         isolate;
    u16          selectedX;
    u16          selectedY;
    u08          selectMult;
    u16          tilesize;
    u32          mapWidth;
    u32          mapHeight;
    u08          mapLayers;
    TileMetadata *tileData;
    TileIndex    *placedTiles;
    u08          flags;
    Button       new_b;
    Button       load_b;
    Button       save_b;
    Button       saveas_b;
    Button       quit_b;
    Button       close_b;
    Button       collision_b;
    Button       animation_b;
    u08          i_button;
    Action       *actions;
    u32          currentAction;
    String       inputBuffer;
    StringView   filename;
    u08          viewScroll;
    u08          cursor;
    bool         confirmed;
}
EditorData;

extern void meInit
(
    EngineData *engine,
    EditorData *editor
);

int32_t meShutdown
(
    EditorData *editor
);

extern void meUpdate
(
    EngineData *engine,
    EditorData *editor
);

extern void mePresent
(
    EngineData *engine,
    EditorData *editor
);

extern void meScroll
(
    EditorData *editor,
    bool       increase
);

extern void meUpdateSelectSize
(
    EditorData *editor,
    bool       increase
);

extern void meProcessButtons
(
    EditorData    *editor,
    RiverControls *controls,
    u32           button,
    bool          isDown
);

extern void meProcessKeys
(
    RiverControls *controls,
    AsciiKey      key,
    bool          isDown
);

extern void meProcessPointer
(
    EngineData *engine,
    u32        x,
    u32        y
);

extern void mePlaceSelectedTiles
(
    EngineData *engine,
    EditorData *editor,
    u16        tileX,
    u16        tileY
);

extern void meChangeState
(
    EditorData *editor,
    u08        nextState
);

extern void meSaveProject
(
    EngineData *engine,
    EditorData *editor
);

extern void meLoadProject
(
    EngineData *engine,
    EditorData *editor
);

extern void meUndo
(
    EditorData *editor
);

extern void meRedo
(
    EditorData *editor
);

extern void meDrawMainMenu
(
    EngineData *engine,
    EditorData *editor
);

extern void mePollMainMenu
(
    EngineData *engine,
    EditorData *editor
);

extern void meDrawEditor
(
    EngineData *engine,
    EditorData *editor
);

extern void mePollEditor
(
    EngineData *engine,
    EditorData *editor
);

extern void meDrawFilePicker
(
    EngineData *engine
);

extern void mePollLoadFile
(
    EngineData *engine,
    EditorData *editor
);

extern void mePollSaveFile
(
    EngineData *engine,
    EditorData *editor
);
