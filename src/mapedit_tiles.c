#include "mapedit_main.h"

#include <math.h>

f_internal void incrementAction
(
    EditorData *editor
){
    if(++editor->currentAction > ME_MAX_ACTIONS - 1)
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
        editor->currentAction = ME_MAX_ACTIONS - 1;
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
    editor->placedTiles[action.map_index] = action.prev_tile;
}

f_internal void readAction_redo
(
    EditorData *editor
){
    Action action = editor->actions[editor->currentAction];
    editor->placedTiles[action.map_index] = action.new_tile;
}

f_internal void writeAction
(
    EditorData *editor,
    u64        map_index,
    TileIndex  new_tile
){
    TileIndex prev_tile = editor->placedTiles[map_index];
    editor->placedTiles[map_index] = new_tile;

    editor->actions[editor->currentAction].stroke_start = editor->lastActionStart;
    editor->actions[editor->currentAction].map_index    = map_index;
    editor->actions[editor->currentAction].prev_tile    = prev_tile;
    editor->actions[editor->currentAction].new_tile     = new_tile;
}

void mePlaceSelectedTiles
(
    EngineData *engine,
    EditorData *editor,
    u16        tileX,
    u16        tileY
){
    bool break_outer = false;
    u64  sliceSize   = editor->mapWidth * editor->mapHeight;
    u64  topLeft     = editor->currentLayer * sliceSize +
                       tileY * editor->mapWidth + tileX;
    u64 maxCurIndex  = sliceSize + editor->currentLayer * sliceSize - 1;

    if(topLeft > maxCurIndex)
    {
        topLeft = maxCurIndex;
    }

    for(u08 y = 0; y < editor->selectMult && !break_outer; ++y)
    {
        if(tileY + y > editor->mapHeight - 1)
        {
            break;
        }

        for(u08 x = 0; x < editor->selectMult; ++x)
        {
            if(tileX + x > editor->mapWidth - 1)
            {
                break;
            }

            u64 index = topLeft + y * editor->mapWidth + x;
            if(index > maxCurIndex)
            {
                break_outer = true;
                break;
            }

            TileIndex new_tile = {0};
            new_tile.x = editor->selectedX + x;
            new_tile.y = editor->selectedY + y;

            u32 prev = editor->currentAction - 1;
            if(editor->currentAction == 0)
            {
                prev = ME_MAX_ACTIONS - 1;
            }
            Action prev_action = editor->actions[prev];

            i64 delta = rvDeltaTime_ns(&prev_action.stroke_start,
                                                 &editor->lastActionStart);

            u64 diff  = index - prev_action.map_index;
            u64 diffY = diff / editor->mapHeight;
            u64 diffX = diff % editor->mapWidth;
            bool backwards = false;

            if(prev_action.map_index > index)
            {
                diff  = prev_action.map_index - index;
                diffY = diff / editor->mapHeight;
                diffX = diff % editor->mapWidth;
                backwards = true;
            }

            if(delta || (!diffX && !diffY))
            {
                goto skipFill;
            }

            // FIXME: actually fill in gaps here, too
            // figure out a way to do selectMult-independent filling... 🤔
            // we need to jump far enough back in the action history for that,
            // selectMult * selectMult. 1^2 is still 1, but for a stencil of 2^2
            // we need to jump back 4.
            if(engine->controls.keymap & ME_BIT_LSHIFT)
            {
                goto skipFill;
            }

            u64 layerSize = editor->mapWidth * editor->mapHeight;

            if(backwards && !diffY && diffX > 1)
            {
                for(u64 i = diffX - 1; i > 0; --i)
                {
                    u64 fillIndex = index + i;
                    writeAction(editor, fillIndex, new_tile);
                    incrementAction(editor);
                }
            }
            else if(!diffY && diffX > 1)
            {
                for(u64 i = diffX - 1; i > 0; --i)
                {
                    u64 fillIndex = index - i;
                    writeAction(editor, fillIndex, new_tile);
                    incrementAction(editor);
                }
            }
            else if(backwards && !diffX && diffY > 1)
            {
                for(u64 i = diffY - 1; i > 0; --i)
                {
                    u64 rows          = i * editor->mapWidth;
                    u64 curLayerIndex = index % layerSize;

                    if(rows > curLayerIndex)
                    {
                        continue;
                    }

                    u64 fillIndex = index + rows;

                    writeAction(editor, fillIndex, new_tile);
                    incrementAction(editor);
                }
            }
            else if(!diffX && diffY > 1)
            {
                for(u64 i = diffY - 1; i > 0; --i)
                {
                    u64 rows          = i * editor->mapWidth;
                    u64 curLayerIndex = index % layerSize;

                    if(rows > layerSize - curLayerIndex)
                    {
                        continue;
                    }

                    u64 fillIndex = index - rows;

                    writeAction(editor, fillIndex, new_tile);
                    incrementAction(editor);
                }
            }

skipFill:
            writeAction(editor, index, new_tile);
            incrementAction(editor);
        }
    }
}

void meUndo
(
    EditorData *editor
){
    Action currentAction;
    Action prevAction;

    if(editor->currentAction == 0)
    {
        prevAction = editor->actions[ME_MAX_ACTIONS - 1];
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

    for(u32 undoCount = 0; undoCount < ME_MAX_ACTIONS; ++undoCount)
    {
        decrementAction(editor);
        currentAction = editor->actions[editor->currentAction];
        if(editor->currentAction == 0)
        {
            prevAction = editor->actions[ME_MAX_ACTIONS - 1];
        }
        else
        {
            prevAction = editor->actions[editor->currentAction - 1];
        }

        readAction_undo(editor);

        editor->currentLayer = (u08)floor((f64)currentAction.map_index /
                                              (editor->mapWidth * editor->mapHeight));

        i64 deltaNS = rvDeltaTime_ns(&prevAction.stroke_start,
                                               &currentAction.stroke_start);
        if(deltaNS != 0)
        {
            break;
        }
    }
}

void meRedo
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
        prevAction = editor->actions[ME_MAX_ACTIONS - 1];
    }
    else
    {
        prevAction = editor->actions[editor->currentAction - 1];
    }

    if(prevAction.stroke_start.s  >= 0 &&
       prevAction.stroke_start.ns >= 0 &&
       rvDeltaTime_ns(&currentAction.stroke_start, &prevAction.stroke_start) > 0
    ){
        return;
    }

    Action nextAction = {0};
    for(u32 undoCount = 0; undoCount < ME_MAX_ACTIONS; ++undoCount)
    {
        currentAction = editor->actions[editor->currentAction];
        if(editor->currentAction >= ME_MAX_ACTIONS - 1)
        {
            nextAction = editor->actions[0];
        }
        else
        {
            nextAction = editor->actions[editor->currentAction + 1];
        }

        readAction_redo(editor);
        editor->currentLayer = (u08)floor((f64)currentAction.map_index /
                                          (editor->mapWidth * editor->mapHeight));
        incrementAction(editor);

        i64 deltaNS = rvDeltaTime_ns(&nextAction.stroke_start,
                                         &currentAction.stroke_start);
        if(deltaNS != 0)
        {
            break;
        }
    }
}
