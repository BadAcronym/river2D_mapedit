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

f_internal void nextButton
(
    EditorData *editor,
    bool       upward
){
    if(!upward)
    {
        goto down;
    }

    if(editor->i_button == 0)
    {
        editor->quit_b.status |= RIVER2D_BIT_HOVER;
    }
    else if(editor->i_button == 2)
    {
        editor->load_b.status &= ~RIVER2D_BIT_HOVER;
        editor->new_b.status  |= RIVER2D_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 3)
    {
        editor->save_b.status &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status |= RIVER2D_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 4)
    {
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status   |= RIVER2D_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 5)
    {
        editor->quit_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status |= RIVER2D_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 5)
    {
        editor->quit_b.status &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status |= RIVER2D_BIT_HOVER;
        editor->i_button = 3;
    }

    return;

down:
    if(editor->i_button == 5)
    {
        return;
    }
    else if(editor->i_button == 0)
    {
        editor->new_b.status |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 1)
    {
        editor->new_b.status  &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 2)
    {
        editor->load_b.status &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 2)
    {
        editor->load_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 3)
    {
        editor->save_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 3)
    {
        editor->save_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status |= RIVER2D_BIT_HOVER;
        editor->i_button = 5;
    }
    else if(editor->i_button == 4)
    {
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status   |= RIVER2D_BIT_HOVER;
        ++editor->i_button;
    }
}

f_internal void pollHovers
(
    EngineData *engine,
    EditorData *editor
){
    if(river2D_insideRect(&engine->controls.pointer, &editor->new_b.area))
    {
        editor->new_b.status    |= RIVER2D_BIT_HOVER;
        editor->load_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->i_button = 1;
        return;
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->load_b.area))
    {
        editor->new_b.status    &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status   |= RIVER2D_BIT_HOVER;
        editor->save_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->i_button = 2;
        return;
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->save_b.area))
    {
        editor->new_b.status    &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status   |= RIVER2D_BIT_HOVER;
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->i_button = 3;
        return;
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->saveas_b.area))
    {
        editor->new_b.status    &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status |= RIVER2D_BIT_HOVER;
        editor->quit_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->i_button = 4;
        return;
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->quit_b.area))
    {
        editor->new_b.status    &= ~RIVER2D_BIT_HOVER;
        editor->load_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->save_b.status   &= ~RIVER2D_BIT_HOVER;
        editor->saveas_b.status &= ~RIVER2D_BIT_HOVER;
        editor->quit_b.status   |= RIVER2D_BIT_HOVER;
        editor->i_button = 5;
        return;
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
       engine->controls.keymap & MAPEDIT_BIT_QUIT
    ){
        engine->running = false;
        return;
    }

    if(editor->previousState && engine->controls.keymap & MAPEDIT_BIT_MENU)
    {
        mapedit_changeState(editor, editor->previousState);
        engine->controls.keymap &= ~MAPEDIT_BIT_MENU;
        return;
    }

    pollHovers(engine, editor);

    if(engine->controls.keymap & MAPEDIT_BIT_UP)
    {
        nextButton(editor, true);
        engine->controls.keymap &= ~MAPEDIT_BIT_UP;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_DOWN)
    {
        nextButton(editor, false);
        engine->controls.keymap &= ~MAPEDIT_BIT_DOWN;
    }

    if(editor->new_b.status & RIVER2D_BIT_HOVER)
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

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM ||
           engine->controls.keymap    & MAPEDIT_BIT_ENTER
        ){
            if(editor->previousState && editor->tiles)
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
    else if(editor->load_b.status & RIVER2D_BIT_HOVER)
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

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM ||
           engine->controls.keymap    & MAPEDIT_BIT_ENTER
        ){
            engine->controls.ascii = 0x00;
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            engine->controls.keymap    &= ~MAPEDIT_BIT_ENTER;
            mapedit_changeState(editor, MAPEDIT_STATE_LOAD);
        }
    }
    else if(editor->quit_b.status & RIVER2D_BIT_HOVER)
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

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM ||
           engine->controls.keymap    & MAPEDIT_BIT_ENTER
        ){
            engine->running = false;
        }
    }
    else if(editor->previousState && editor->save_b.status & RIVER2D_BIT_HOVER)
    {
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

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM ||
           engine->controls.keymap    & MAPEDIT_BIT_ENTER
        ){
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            engine->controls.keymap    &= ~MAPEDIT_BIT_ENTER;
            mapedit_saveProject(engine, editor);
        }
    }
    else if(editor->previousState && editor->saveas_b.status & RIVER2D_BIT_HOVER)
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->saveas_b.area.lowRight.x - editor->saveas_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);

        comp.src        = &engine->planes[MAPEDIT_PLANE_HIGHLIGHT];
        comp.offsetDstX = (uint32_t)(editor->saveas_b.area.upLeft.x * fX);
        comp.offsetDstY = (uint32_t)(editor->saveas_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (uint32_t)(length * fX);
        comp.cropHeight = 5;

        river2D_compositeImage(engine, &comp);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            engine->controls.keymap    &= ~MAPEDIT_BIT_ENTER;
            mapedit_changeState(editor, MAPEDIT_STATE_SAVEAS);
        }
    }
    else if(!editor->i_button)
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
        if(engine->controls.keymap & (MAPEDIT_BIT_LAYER0 << i))
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

    if(engine->controls.keymap & MAPEDIT_BIT_TILEPICKER)
    {
        editor->editorflags     ^= MAPEDIT_FLAG_BIT_TILEPICKER;
        engine->controls.keymap &= ~MAPEDIT_BIT_TILEPICKER;
    }

    float fX = (float)engine->backbuffer.width;
    float fY = (float)engine->backbuffer.height;

    if(editor->editorflags & MAPEDIT_FLAG_BIT_TILEPICKER)
    {
        if(engine->controls.keymap & MAPEDIT_BIT_MENU)
        {
            engine->controls.keymap &= ~MAPEDIT_BIT_MENU;
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
        float max_h = (float)engine->backbuffer.height * 0.8f - 3 * editor->tilesize;

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

            if(engine->controls.keymap & MAPEDIT_BIT_DECREASE)
            {
                mapedit_updateSelectSize(editor, false);
                engine->controls.keymap &= ~MAPEDIT_BIT_DECREASE;
            }
            if(engine->controls.keymap & MAPEDIT_BIT_INCREASE)
            {
                mapedit_updateSelectSize(editor, true);
                engine->controls.keymap &= ~MAPEDIT_BIT_INCREASE;
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
    else if(engine->controls.keymap & MAPEDIT_BIT_UNDO &&
            engine->controls.keymap & MAPEDIT_BIT_LCTRL
    ){
        mapedit_undo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_UNDO;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_REDO &&
            engine->controls.keymap & MAPEDIT_BIT_LCTRL
    ){
        mapedit_redo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_REDO;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_MENU)
    {
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_MENU;
        return;
    }
    else if((engine->controls.keymap & MAPEDIT_BIT_LSHIFT  ||
             engine->controls.keymap & MAPEDIT_BIT_RSHIFT) &&
             engine->controls.keymap & MAPEDIT_BIT_LCTRL   &&
             engine->controls.keymap & MAPEDIT_BIT_SAVE
    ){
        engine->controls.ascii = 0x00;
        mapedit_changeState(editor, MAPEDIT_STATE_SAVEAS);
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LCTRL &&
            engine->controls.keymap & MAPEDIT_BIT_SAVE
    ){
        mapedit_saveProject(engine, editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_SAVE;
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
    comp.offsetDstX = (uint32_t)(0.025f * (float)engine->backbuffer.width);
    comp.offsetDstY = (uint32_t)(0.4f * (float)engine->backbuffer.height);
    comp.cropWidth  = engine->planes[MAPEDIT_PLANE_CURRENTFILE].width;
    comp.cropHeight = engine->planes[MAPEDIT_PLANE_CURRENTFILE].height;

    river2D_compositeImage(engine, &comp);
}

f_internal void decreaseCursor
(
    EditorData *editor
){
    if(editor->cursor == 0)
    {
        return;
    }

    --editor->cursor;
}

f_internal void increaseCursor
(
    EditorData *editor
){
    if(editor->cursor == 255)
    {
        return;
    }

    ++editor->cursor;
}

f_internal void pollInput
(
    EngineData *engine,
    EditorData *editor
){
    editor->inputBuffer.data[editor->cursor] = RIVER2D_ASCII_CURSOR;

    if(engine->controls.keymap & MAPEDIT_BIT_MENU)
    {
        mapedit_changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_MENU;
        return;
    }

    if(engine->controls.ascii)
    {
        editor->inputBuffer.data[editor->cursor] = engine->controls.ascii;
        engine->controls.ascii = 0x00;

        increaseCursor(editor);
    }

    if(engine->controls.keymap & MAPEDIT_BIT_BACKSPACE)
    {
        editor->inputBuffer.data[editor->cursor] = '\0';

        decreaseCursor(editor);

        engine->controls.keymap &= ~MAPEDIT_BIT_BACKSPACE;
    }

    if(engine->controls.keymap & MAPEDIT_BIT_ENTER)
    {
        editor->inputBuffer.data[editor->cursor] = '\0';

        editor->confirmed = true;
        engine->controls.keymap &= ~MAPEDIT_BIT_ENTER;
    }
}

f_internal void drawTextBuffer
(
    EngineData *engine,
    EditorData *editor
){
    rvLoadTextSettings set = {0};
    set.image    = &engine->planes[MAPEDIT_PLANE_CURRENTFILE];
    set.sv       = (StringView*)&editor->inputBuffer;
    set.font     = MAPEDIT_PLANE_FONT16,
    set.charsize = 16;
    set.spacing  = 1;

    river2D_loadText(engine, &set);
}

void mapedit_pollLoadFile
(
    EngineData *engine,
    EditorData *editor
){
    pollInput(engine, editor);

    if(editor->confirmed)
    {
        mapedit_loadProject(engine, editor);
        mapedit_changeState(editor, MAPEDIT_STATE_EDIT);
        editor->confirmed = false;
        return;
    }

    drawTextBuffer(engine, editor);
}

void mapedit_pollSaveFile
(
    EngineData *engine,
    EditorData *editor
){
    pollInput(engine, editor);

    if(editor->confirmed)
    {
        mapedit_saveProject(engine, editor);
        mapedit_changeState(editor, MAPEDIT_STATE_EDIT);
        editor->confirmed = false;
        return;
    }

    drawTextBuffer(engine, editor);
}
