#include "mapedit_main.h"

void mapedit_drawMainMenu
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[MAPEDIT_PLANE_BACKGROUND];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RIVER2D_PICTOP_OVER;
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_BACKGROUND].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_BACKGROUND].height;

    river2D_compositeImage(engine, &comp);

    if(!editor->previousState)
    {
        comp.src        = &engine->planes[MAPEDIT_PLANE_MAINMENU];
        comp.cropWidth  = engine->planes[MAPEDIT_PLANE_MAINMENU].width;
        comp.cropHeight = engine->planes[MAPEDIT_PLANE_MAINMENU].height;

        river2D_compositeImage(engine, &comp);
    }
    else
    {
        comp.src        = &engine->planes[MAPEDIT_PLANE_PAUSEMENU];
        comp.cropWidth  = engine->planes[MAPEDIT_PLANE_PAUSEMENU].width;
        comp.cropHeight = engine->planes[MAPEDIT_PLANE_PAUSEMENU].height;

        river2D_compositeImage(engine, &comp);
    }

    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        comp.src        = &engine->planes[MAPEDIT_PLANE_ICON_SAVED];
        comp.offsetDstX = 16;
        comp.offsetDstY = 16;
        comp.cropWidth  = engine->planes[MAPEDIT_PLANE_ICON_SAVED].width;
        comp.cropHeight = engine->planes[MAPEDIT_PLANE_ICON_SAVED].height;

        river2D_compositeImage(engine, &comp);
    }
}

void mapedit_pollMainMenu
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RIVER2D_PICTOP_OVER;

    if(engine->controls.keymap & MAPEDIT_BIT_LCTRL &&
       engine->controls.keymap & MAPEDIT_BIT_Q
    ){
        engine->running = false;
        return;
    }

    if(engine->controls.keymap &  MAPEDIT_BIT_ESCAPE &&
       editor->previousState   != MAPEDIT_STATE_NULL
    ){
        mapedit_changeState(editor, editor->previousState);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    if(river2D_insideRect(&engine->controls.pointer, &editor->new_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->new_b.area.lowRight.x - editor->new_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);

        comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
        comp.offsetDstX = (uint32_t)(editor->new_b.area.upLeft.x * fX);
        comp.offsetDstY = (uint32_t)(editor->new_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (uint32_t)(length * fX);
        comp.cropHeight = 5;

        river2D_compositeImage(engine, &comp);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            if(editor->previousState != MAPEDIT_STATE_NULL && editor->tiles)
            {
                uint64_t tilecount = editor->layers *
                                     editor->mapHeight * editor->mapWidth;
                for(uint32_t i = 0; i < tilecount; ++i)
                {
                    editor->tiles[i].flags |= MAPEDIT_BIT_INVALID;
                }
            }

            mapedit_changeState(editor, MAPEDIT_STATE_EDIT);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->load_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->load_b.area.lowRight.x - editor->load_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);

        comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
        comp.offsetDstX = (uint32_t)(editor->load_b.area.upLeft.x * fX);
        comp.offsetDstY = (uint32_t)(editor->load_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (uint32_t)(length * fX);
        comp.cropHeight = 5;

        river2D_compositeImage(engine, &comp);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            mapedit_changeState(editor, MAPEDIT_STATE_LOAD);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->quit_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->quit_b.area.lowRight.x - editor->quit_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);

        comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
        comp.offsetDstX = (uint32_t)(editor->quit_b.area.upLeft.x * fX);
        comp.offsetDstY = (uint32_t)(editor->quit_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (uint32_t)(length * fX);
        comp.cropHeight = 5;

        river2D_compositeImage(engine, &comp);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            engine->running = false;
        }
    }
    else if(editor->previousState &&
            river2D_insideRect(&engine->controls.pointer, &editor->save_b.area)
    ){
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->save_b.area.lowRight.x - editor->save_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);

        comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
        comp.offsetDstX = (uint32_t)(editor->save_b.area.upLeft.x * fX);
        comp.offsetDstY = (uint32_t)(editor->save_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (uint32_t)(length * fX);
        comp.cropHeight = 5;

        river2D_compositeImage(engine, &comp);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            mapedit_saveProject(engine, editor);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);

        #ifdef DEBUG
        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            fprintf(stderr, "clicked @ X: %f Y: %f\n",
                    engine->controls.pointer.x, engine->controls.pointer.y);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
        #endif
    }
}

void mapedit_drawEditor
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[MAPEDIT_PLANE_VOID];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RIVER2D_PICTOP_OVER;
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_VOID].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_VOID].height;

    river2D_compositeImage(engine, &comp);

    for(uint8_t i = 0; i < editor->layers; ++i)
    {
        if(engine->controls.keymap & (MAPEDIT_BIT_0 << i))
        {
            editor->currentLayer = i;
            editor->isolate      = engine->controls.keymap & MAPEDIT_BIT_LSHIFT;
        }
    }

    uint8_t startLayer = editor->isolate ? editor->currentLayer : 0;
    uint8_t endLayer   = editor->isolate ? editor->currentLayer + 1 : editor->layers;

    for(uint8_t z = startLayer; z < endLayer; ++z)
    {
        for(uint32_t y = 0; y < editor->mapHeight; ++y)
        {
            for(uint32_t x = 0; x < editor->mapWidth; ++x)
            {
                uint64_t index = z * editor->mapWidth * editor->mapHeight +
                                 y * editor->mapWidth + x;
                if(!(editor->tiles[index].flags & MAPEDIT_BIT_INVALID))
                {
                    comp.src        = &engine->planes[MAPEDIT_PLANE_TILESHEET];
                    comp.offsetSrcX = editor->tiles[index].x * editor->tilesize;
                    comp.offsetSrcY = editor->tiles[index].y * editor->tilesize;
                    comp.offsetDstX = x * editor->tilesize;
                    comp.offsetDstY = y * editor->tilesize;
                    comp.cropWidth  = editor->tilesize;
                    comp.cropHeight = editor->tilesize;

                    river2D_compositeImage(engine, &comp);
                }
            }
        }
    }

    char layerStr[2];
    snprintf(layerStr, 2, "%hhu", editor->currentLayer);

    StringView layer_sv = cstr_sv(layerStr);

    rvLoadTextSettings set = {0};
    set.sv       = &layer_sv;
    set.image    = &engine->planes[MAPEDIT_PLANE_CURRENTLAYER];
    set.font     = MAPEDIT_PLANE_FONT16;
    set.charsize = 16;
    set.spacing  = 1;

    river2D_loadText(engine, &set);

    comp.src        = &engine->planes[MAPEDIT_PLANE_CURRENTLAYER];
    comp.offsetSrcX = 0;
    comp.offsetSrcY = 0;
    comp.offsetDstX = (uint32_t)(0.95f * (float)engine->backbuffer.width);
    comp.offsetDstY = (uint32_t)(0.025f * (float)engine->backbuffer.width);
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_CURRENTLAYER].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_CURRENTLAYER].height;

    river2D_compositeImage(engine, &comp);

    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        comp.src        = &engine->planes[MAPEDIT_PLANE_ICON_SAVED];
        comp.cropWidth  = engine->planes[MAPEDIT_PLANE_ICON_SAVED].width;
        comp.cropHeight = engine->planes[MAPEDIT_PLANE_ICON_SAVED].height;
        comp.offsetDstX = 16;
        comp.offsetDstY = 16;

        river2D_compositeImage(engine, &comp);
    }
}

void mapedit_pollEditor
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RIVER2D_PICTOP_OVER;

    if(engine->controls.keymap & MAPEDIT_BIT_T)
    {
        editor->editorflags     ^= MAPEDIT_FLAG_BIT_TILEPICKER;
        engine->controls.keymap &= ~MAPEDIT_BIT_T;
    }

    float fX = (float)engine->backbuffer.width;
    float fY = (float)engine->backbuffer.height;

    if(editor->editorflags & MAPEDIT_FLAG_BIT_TILEPICKER)
    {
        if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
        {
            engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
            editor->editorflags     &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
            return;
        }

        comp.src        = &engine->planes[MAPEDIT_PLANE_BACKGROUND];
        comp.offsetDstX = engine->backbuffer.width  / 10;
        comp.offsetDstY = engine->backbuffer.height / 10;
        comp.cropWidth  = (uint32_t)((float)engine->backbuffer.width  / 1.25f);
        comp.cropHeight = (uint32_t)((float)engine->backbuffer.height / 1.25f);

        river2D_compositeImage(engine, &comp);

        comp.src        = &engine->planes[MAPEDIT_PLANE_SELECTTILE];
        comp.offsetDstX = 0;
        comp.offsetDstY = 0;
        comp.cropWidth  = engine->planes[MAPEDIT_PLANE_SELECTTILE].width;
        comp.cropHeight = engine->planes[MAPEDIT_PLANE_SELECTTILE].height;

        river2D_compositeImage(engine, &comp);

        uint32_t sheet_width  = engine->planes[MAPEDIT_PLANE_TILESHEET].width;
        uint32_t sheet_height = engine->planes[MAPEDIT_PLANE_TILESHEET].height;

        float max_w = (float)engine->backbuffer.width  * 0.8f - 2 * editor->tilesize;
        float max_h = (float)engine->backbuffer.height * 0.8f - 2 * editor->tilesize;

        if((float)sheet_width > max_w)
        {
            sheet_width = (uint32_t)max_w;
        }
        if((float)sheet_height > max_h)
        {
            sheet_height = (uint32_t)max_h;
        }

        comp.src        = &engine->planes[MAPEDIT_PLANE_TILESHEET];
        comp.offsetSrcX = 0;
        comp.offsetSrcY = editor->viewScroll * editor->tilesize;
        comp.offsetDstX = engine->backbuffer.width  / 10 + editor->tilesize;
        comp.offsetDstY = engine->backbuffer.height / 10 + editor->tilesize;
        comp.cropWidth  = sheet_width;
        comp.cropHeight = sheet_height;

        river2D_compositeImage(engine, &comp);

        Rect tiles = {0};
        tiles.upLeft.x   = 0.095f + (float)((float)editor->tilesize / fX);
        tiles.upLeft.y   = 0.095f + (float)((float)editor->tilesize / fY);
        tiles.lowRight.x = tiles.upLeft.x + (float)sheet_width / fX;
        tiles.lowRight.y = tiles.upLeft.y + (float)sheet_height / fY;

        if(river2D_insideRect(&engine->controls.pointer, &editor->close_b.area))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            float length = editor->close_b.area.lowRight.x -
                           editor->close_b.area.upLeft.x;

            comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
            comp.offsetSrcX = 0;
            comp.offsetSrcY = 0;
            comp.offsetDstX = (uint32_t)(editor->close_b.area.upLeft.x * fX);
            comp.offsetDstY = (uint32_t)(editor->close_b.area.upLeft.y * fY + 20);
            comp.cropWidth  = (uint32_t)(length * fX);
            comp.cropHeight = 5;

            river2D_compositeImage(engine, &comp);

            if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
            {
                editor->editorflags        &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
                engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            }
        }
        else if(river2D_insideRect(&engine->controls.pointer, &tiles))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_NULL]);

            float    deltaX = engine->controls.pointer.x - tiles.upLeft.x;
            float    deltaY = engine->controls.pointer.y - tiles.upLeft.y;
            uint8_t  tileX  = (uint8_t)(deltaX * fX / editor->tilesize);
            uint8_t  tileY  = (uint8_t)(deltaY * fY / editor->tilesize);
            uint16_t sheetX = (uint16_t)(sheet_width  / editor->tilesize);
            uint16_t sheetY = (uint16_t)(sheet_height / editor->tilesize);

            if(tileX + editor->selectMult > sheetX)
            {
                tileX = (uint8_t)(sheetX - editor->selectMult);
            }
            if(tileY + editor->selectMult > sheetY)
            {
                tileY = (uint8_t)(sheetY - editor->selectMult);
            }

            if(engine->controls.keymap & MAPEDIT_BIT_MINUS)
            {
                mapedit_updateSelectSize(editor, false);
                engine->controls.keymap &= ~MAPEDIT_BIT_MINUS;
            }
            if(engine->controls.keymap & MAPEDIT_BIT_EQUAL)
            {
                mapedit_updateSelectSize(editor, true);
                engine->controls.keymap &= ~MAPEDIT_BIT_EQUAL;
            }

            comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
            comp.offsetDstX = (uint32_t)(fX * (tiles.upLeft.x + 0.0055f) +
                                              tileX * editor->tilesize);
            comp.offsetDstY = (uint32_t)(fY * (tiles.upLeft.y + 0.006f) +
                                              tileY * editor->tilesize);
            comp.cropWidth  = editor->tilesize * editor->selectMult;
            comp.cropHeight = editor->tilesize * editor->selectMult;

            river2D_compositeImage(engine, &comp);

            if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
            {
                editor->selectedX           = tileX;
                editor->selectedY           = tileY + editor->viewScroll;
                editor->editorflags        &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
                engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            }
        }
        else
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);
        }

        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_Z &&
            engine->controls.keymap & MAPEDIT_BIT_LCTRL
    ){
        mapedit_undo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_Z;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_Y &&
            engine->controls.keymap & MAPEDIT_BIT_LCTRL
    ){
        mapedit_redo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_Y;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LCTRL &&
            engine->controls.keymap & MAPEDIT_BIT_S
    ){
        mapedit_saveProject(engine, editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_S;
        return;
    }

    uint16_t tileX = (uint16_t)(engine->controls.pointer.x * fX  / editor->tilesize);
    uint16_t tileY = (uint16_t)(engine->controls.pointer.y * fY / editor->tilesize);

    uint8_t modX = tileX % editor->selectMult;
    uint8_t modY = tileY % editor->selectMult;

    if(engine->controls.keymap & MAPEDIT_BIT_LSHIFT)
    {
        if(modX)
        {
            tileX -= modX;
        }
        if(modY)
        {
            tileY -= modY;
        }
    }

    comp.src        = &engine->planes[MAPEDIT_PLANE_TILESHEET];
    comp.dst        = &engine->backbuffer;
    comp.offsetSrcX = editor->selectedX * editor->tilesize;
    comp.offsetSrcY = editor->selectedY * editor->tilesize;
    comp.offsetDstX = tileX * editor->tilesize;
    comp.offsetDstY = tileY * editor->tilesize;
    comp.cropWidth  = editor->tilesize  * editor->selectMult;
    comp.cropHeight = editor->tilesize  * editor->selectMult;

    river2D_compositeImage(engine, &comp);

    //if(river2D_insideRect(&engine->controls.pointer, &editor->button))
    // {
    //     river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE]);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            mapedit_placeSelectedTiles(editor, tileX, tileY);
        }
    }
}

void mapedit_drawFilePicker
(
    EngineData *engine
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[MAPEDIT_PLANE_BACKGROUND];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RIVER2D_PICTOP_OVER;
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_BACKGROUND].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_BACKGROUND].height;

    river2D_compositeImage(engine, &comp);

    comp.src = &engine->planes[MAPEDIT_PLANE_CURRENTFILE];
    comp.offsetDstX = (uint32_t)(0.2f * (float)engine->backbuffer.width);
    comp.offsetDstY = (uint32_t)(0.4f * (float)engine->backbuffer.height);
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_CURRENTFILE].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_CURRENTFILE].height;

    river2D_compositeImage(engine, &comp);
}

void mapedit_pollFilePicker
(
    EngineData *engine,
    EditorData *editor
){
    if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    if(engine->controls.keymap & MAPEDIT_BIT_BACKSPACE)
    {
        sv_trim((StringView*)&editor->filename, 1, SV_RIGHT);
        engine->controls.keymap &= ~MAPEDIT_BIT_BACKSPACE;
    }

    bool shift = (engine->controls.keymap & MAPEDIT_BIT_LSHIFT) ||
                 (engine->controls.keymap & MAPEDIT_BIT_RSHIFT);

    for(uint8_t i = 0; i < 26; ++i)
    {
        if(engine->controls.keymap & (MAPEDIT_BIT_A << i))
        {
            engine->controls.keymap &= ~(MAPEDIT_BIT_A << i);
            if(shift)
            {
                editor->filename.data[editor->cursor] = 0x41 + i;
                continue;
            }

            editor->filename.data[editor->cursor] = 0x61 + i;
        }
    }

    // TODO: handle:
    // periods,
    // slashes,
    // dashes,
    // underscores,
    // brackets

    if(engine->controls.keymap & MAPEDIT_BIT_ENTER)
    {
        editor->confirmed = true;
        engine->controls.keymap &= ~MAPEDIT_BIT_ENTER;
    }

    if(editor->confirmed)
    {
        mapedit_loadProject(engine, editor);
        mapedit_changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    rvLoadTextSettings set = {0};
    set.image    = &engine->planes[MAPEDIT_PLANE_CURRENTFILE];
    set.sv       = (StringView*)&editor->filename;
    set.font     = MAPEDIT_PLANE_FONT16,
    set.charsize = 16;
    set.spacing  = 1;

    river2D_loadText(engine, &set);
}
