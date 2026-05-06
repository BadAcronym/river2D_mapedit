#include "mapedit_main.h"

#include <math.h>

f_internal void incrementAction
(
    EditorData *editor
){
    if(++editor->currentAction > MAPEDIT_MAX_ACTIONS - 1)
    {
        editor->currentAction = 0;
    }
}

f_internal void decrementAction
(
    EditorData *editor
){
    if(editor->currentAction == 0)
    {
        editor->currentAction = MAPEDIT_MAX_ACTIONS - 1;
    }
    else
    {
        --editor->currentAction;
    }
}

f_internal void readAction_undo
(
    EditorData *editor
){
    Action action = editor->actions[editor->currentAction];
    editor->tiles[action.map_index] = action.prev_tile;
}

f_internal void readAction_redo
(
    EditorData *editor
){
    Action action = editor->actions[editor->currentAction];
    editor->tiles[action.map_index] = action.new_tile;
}

f_internal void writeAction
(
    EditorData   *editor,
    uint64_t     map_index,
    Tile         new_tile
){
    Tile prev_tile = editor->tiles[map_index];
    editor->tiles[map_index] = new_tile;

    editor->actions[editor->currentAction].stroke_start = editor->lastActionStart;
    editor->actions[editor->currentAction].map_index    = map_index;
    editor->actions[editor->currentAction].prev_tile    = prev_tile;
    editor->actions[editor->currentAction].new_tile     = new_tile;
}

void mapedit_placeSelectedTiles
(
    EditorData *editor,
    uint16_t   tileX,
    uint16_t   tileY
){
    bool     break_outer = false;
    uint64_t sliceSize   = editor->mapWidth * editor->mapHeight;
    uint64_t topLeft     = editor->currentLayer * sliceSize +
                           tileY * editor->mapWidth + tileX;
    uint64_t maxCurIndex = sliceSize + editor->currentLayer * sliceSize - 1;

    if(topLeft > maxCurIndex)
    {
        topLeft = maxCurIndex;
    }

    for(uint8_t y = 0; y < editor->selectMult && !break_outer; ++y)
    {
        if(tileY + y > editor->mapHeight - 1)
        {
            break;
        }

        for(uint8_t x = 0; x < editor->selectMult; ++x)
        {
            if(tileX + x > editor->mapWidth - 1)
            {
                break;
            }

            uint64_t index = topLeft + y * editor->mapWidth + x;
            if(index > maxCurIndex)
            {
                break_outer = true;
                break;
            }

            if(editor->tiles[index].x == editor->selectedX + x &&
               editor->tiles[index].y == editor->selectedY + y
            ){
                continue;
            }

            Tile new_tile = {0};
            new_tile.x = editor->selectedX + x;
            new_tile.y = editor->selectedY + y;

            writeAction(editor, index, new_tile);
            incrementAction(editor);
        }
    }
}

void mapedit_undo
(
    EditorData *editor
){
    Action currentAction;
    Action prevAction;

    if(editor->currentAction == 0)
    {
        prevAction = editor->actions[MAPEDIT_MAX_ACTIONS - 1];
    }
    else
    {
        prevAction = editor->actions[editor->currentAction - 1];
    }

    if(prevAction.stroke_start.s  == INT64_MIN &&
       prevAction.stroke_start.ns == INT64_MIN
    ){
        return;
    }

    for(uint32_t undoCount = 0; undoCount < MAPEDIT_MAX_ACTIONS; ++undoCount)
    {
        decrementAction(editor);
        currentAction = editor->actions[editor->currentAction];
        if(editor->currentAction == 0)
        {
            prevAction = editor->actions[MAPEDIT_MAX_ACTIONS - 1];
        }
        else
        {
            prevAction = editor->actions[editor->currentAction - 1];
        }

        readAction_undo(editor);

        editor->currentLayer = (uint8_t)floor((double)currentAction.map_index /
                                              (editor->mapWidth * editor->mapHeight));

        int64_t deltaNS = river2D_deltaTime_ns(&prevAction.stroke_start,
                                               &currentAction.stroke_start);
        if(deltaNS != 0)
        {
            break;
        }
    }
}

void mapedit_redo
(
    EditorData *editor
){
    Action currentAction = editor->actions[editor->currentAction];
    if(currentAction.stroke_start.s  == INT64_MIN &&
       currentAction.stroke_start.ns == INT64_MIN
    ){
        return;
    }

    Action prevAction;
    if(editor->currentAction == 0)
    {
        prevAction = editor->actions[MAPEDIT_MAX_ACTIONS - 1];
    }
    else
    {
        prevAction = editor->actions[editor->currentAction - 1];
    }

    if(prevAction.stroke_start.s  >= 0 &&
       prevAction.stroke_start.ns >= 0 &&
       river2D_deltaTime_ns(&currentAction.stroke_start, &prevAction.stroke_start) > 0
    ){
        return;
    }

    Action nextAction;
    for(uint32_t undoCount = 0; undoCount < MAPEDIT_MAX_ACTIONS; ++undoCount)
    {
        currentAction = editor->actions[editor->currentAction];
        if(editor->currentAction >= MAPEDIT_MAX_ACTIONS - 1)
        {
            nextAction = editor->actions[0];
        }
        else
        {
            nextAction = editor->actions[editor->currentAction + 1];
        }

        readAction_redo(editor);
        editor->currentLayer = (uint8_t)floor((double)currentAction.map_index /
                                              (editor->mapWidth * editor->mapHeight));
        incrementAction(editor);

        int64_t deltaNS = river2D_deltaTime_ns(&nextAction.stroke_start,
                                               &currentAction.stroke_start);
        if(deltaNS != 0)
        {
            break;
        }
    }
}
