#include "mapedit_main.h"
#include "me_print_macros.h"

f_internal void updateFPS
(
    EngineData *engine,
    EditorData *editor
){
    u64 deltaNS = rvDeltaTime_now_ns(&editor->lastFPSTime);
    if(deltaNS < 320000000UL)
    {
        return;
    }

    f32 lastFPS = (f32)editor->runningFrames * 1e9f / (f32)deltaNS;

    char fpsStr[14];
    snprintf(fpsStr, 14, "FPS: %.1f", lastFPS);
    StringView fps_sv = pdCstrSV(fpsStr);

    rvLoadTextSettings textSet = {0};
    textSet.image    = &engine->planes[ME_PLANE_FPS];
    textSet.sv       = &fps_sv;
    textSet.font     = ME_PLANE_FONT16;
    textSet.charsize = 16;
    textSet.spacing  = 1;

    rvLoadText(engine, &textSet);

    editor->lastFPSTime   = rvQueryTime();
    editor->runningFrames = 0;
}

void meDrawMainMenu
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[ME_PLANE_BACKGROUND];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.cropWidth  = engine->planes[ME_PLANE_BACKGROUND].width;
    comp.cropHeight = engine->planes[ME_PLANE_BACKGROUND].height;

    rvCompositeImage(engine, &comp);

    if(!editor->previousState)
    {
        comp.src        = &engine->planes[ME_PLANE_MAINMENU];
        comp.cropWidth  = engine->planes[ME_PLANE_MAINMENU].width;
        comp.cropHeight = engine->planes[ME_PLANE_MAINMENU].height;

        rvCompositeImage(engine, &comp);
    }
    else
    {
        comp.src        = &engine->planes[ME_PLANE_PAUSEMENU];
        comp.cropWidth  = engine->planes[ME_PLANE_PAUSEMENU].width;
        comp.cropHeight = engine->planes[ME_PLANE_PAUSEMENU].height;

        rvCompositeImage(engine, &comp);
    }

    f32 deltaMS = rvDeltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        comp.src        = &engine->planes[ME_PLANE_ICON_SAVED];
        comp.offsetDstX = 16;
        comp.offsetDstY = 16;
        comp.cropWidth  = engine->planes[ME_PLANE_ICON_SAVED].width;
        comp.cropHeight = engine->planes[ME_PLANE_ICON_SAVED].height;

        rvCompositeImage(engine, &comp);
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
        editor->quit_b.status |= RV_BIT_HOVER;
    }
    else if(editor->i_button == 2)
    {
        editor->load_b.status &= ~RV_BIT_HOVER;
        editor->new_b.status  |= RV_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 3)
    {
        editor->save_b.status &= ~RV_BIT_HOVER;
        editor->load_b.status |= RV_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 4)
    {
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->save_b.status   |= RV_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 5)
    {
        editor->quit_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status |= RV_BIT_HOVER;
        --editor->i_button;
    }
    else if(editor->i_button == 5)
    {
        editor->quit_b.status &= ~RV_BIT_HOVER;
        editor->save_b.status |= RV_BIT_HOVER;
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
        editor->new_b.status |= RV_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 1)
    {
        editor->new_b.status  &= ~RV_BIT_HOVER;
        editor->load_b.status |= RV_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 2)
    {
        editor->load_b.status &= ~RV_BIT_HOVER;
        editor->save_b.status |= RV_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 2)
    {
        editor->load_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status |= RV_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->previousState && editor->i_button == 3)
    {
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status |= RV_BIT_HOVER;
        ++editor->i_button;
    }
    else if(editor->i_button == 3)
    {
        editor->save_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status |= RV_BIT_HOVER;
        editor->i_button = 5;
    }
    else if(editor->i_button == 4)
    {
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   |= RV_BIT_HOVER;
        ++editor->i_button;
    }
}

f_internal void pollHovers
(
    EngineData *engine,
    EditorData *editor
){
    if(rvInsideRect(&engine->controls.pointer, &editor->new_b.area))
    {
        editor->new_b.status    |= RV_BIT_HOVER;
        editor->load_b.status   &= ~RV_BIT_HOVER;
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   &= ~RV_BIT_HOVER;
        return;
    }
    else if(rvInsideRect(&engine->controls.pointer, &editor->load_b.area))
    {
        editor->new_b.status    &= ~RV_BIT_HOVER;
        editor->load_b.status   |= RV_BIT_HOVER;
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   &= ~RV_BIT_HOVER;
        return;
    }
    else if(rvInsideRect(&engine->controls.pointer, &editor->save_b.area))
    {
        editor->new_b.status    &= ~RV_BIT_HOVER;
        editor->load_b.status   &= ~RV_BIT_HOVER;
        editor->save_b.status   |= RV_BIT_HOVER;
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   &= ~RV_BIT_HOVER;
        return;
    }
    else if(rvInsideRect(&engine->controls.pointer, &editor->saveas_b.area))
    {
        editor->new_b.status    &= ~RV_BIT_HOVER;
        editor->load_b.status   &= ~RV_BIT_HOVER;
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status |= RV_BIT_HOVER;
        editor->quit_b.status   &= ~RV_BIT_HOVER;
        return;
    }
    else if(rvInsideRect(&engine->controls.pointer, &editor->quit_b.area))
    {
        editor->new_b.status    &= ~RV_BIT_HOVER;
        editor->load_b.status   &= ~RV_BIT_HOVER;
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   |= RV_BIT_HOVER;
        return;
    }
    else if(!editor->i_button)
    {
        editor->new_b.status    &= ~RV_BIT_HOVER;
        editor->load_b.status   &= ~RV_BIT_HOVER;
        editor->save_b.status   &= ~RV_BIT_HOVER;
        editor->saveas_b.status &= ~RV_BIT_HOVER;
        editor->quit_b.status   &= ~RV_BIT_HOVER;
    }
}

void mePollMainMenu
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RV_PICTOP_OVER;

    if(engine->controls.keymap & ME_BIT_LCTRL &&
       engine->controls.keymap & ME_BIT_QUIT
    ){
        engine->running = false;
        return;
    }

    if(editor->previousState && engine->controls.keymap & ME_BIT_MENU)
    {
        meChangeState(editor, editor->previousState);
        engine->controls.keymap &= ~ME_BIT_MENU;
        return;
    }

    pollHovers(engine, editor);

    if(engine->controls.keymap & ME_BIT_UP)
    {
        nextButton(editor, true);
        engine->controls.keymap &= ~ME_BIT_UP;
    }
    else if(engine->controls.keymap & ME_BIT_DOWN)
    {
        nextButton(editor, false);
        engine->controls.keymap &= ~ME_BIT_DOWN;
    }

    if(editor->new_b.status & RV_BIT_HOVER)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->new_b.area.lowRight.x - editor->new_b.area.upLeft.x;
        f32 fX     = (f32)(engine->backbuffer.width);
        f32 fY     = (f32)(engine->backbuffer.height);

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetDstX = (u32)(editor->new_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->new_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM ||
           engine->controls.keymap    & ME_BIT_ENTER
        ){
            if(editor->previousState && editor->placedTiles)
            {
                u64 tilecount = editor->mapLayers *
                                     editor->mapHeight * editor->mapWidth;

                for(u64 i = 0; i < tilecount; ++i)
                {
                    editor->placedTiles[i].x = U16MAX;
                }
            }

            meChangeState(editor, ME_STATE_EDIT);
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
        }
    }
    else if(editor->load_b.status & RV_BIT_HOVER)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->load_b.area.lowRight.x - editor->load_b.area.upLeft.x;
        f32 fX     = (f32)(engine->backbuffer.width);
        f32 fY     = (f32)(engine->backbuffer.height);

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetDstX = (u32)(editor->load_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->load_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM ||
           engine->controls.keymap    & ME_BIT_ENTER
        ){
            engine->controls.ascii = 0x00;
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
            engine->controls.keymap    &= ~ME_BIT_ENTER;
            meChangeState(editor, ME_STATE_LOAD);
        }
    }
    else if(editor->quit_b.status & RV_BIT_HOVER)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->quit_b.area.lowRight.x - editor->quit_b.area.upLeft.x;
        f32 fX     = (f32)(engine->backbuffer.width);
        f32 fY     = (f32)(engine->backbuffer.height);

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetDstX = (u32)(editor->quit_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->quit_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM ||
           engine->controls.keymap    & ME_BIT_ENTER
        ){
            engine->running = false;
        }
    }
    else if(editor->previousState && editor->save_b.status & RV_BIT_HOVER)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->save_b.area.lowRight.x - editor->save_b.area.upLeft.x;
        f32 fX     = (f32)(engine->backbuffer.width);
        f32 fY     = (f32)(engine->backbuffer.height);

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetDstX = (u32)(editor->save_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->save_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM ||
           engine->controls.keymap    & ME_BIT_ENTER
        ){
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
            engine->controls.keymap    &= ~ME_BIT_ENTER;
            meSaveProject(engine, editor);
        }
    }
    else if(editor->previousState && editor->saveas_b.status & RV_BIT_HOVER)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->saveas_b.area.lowRight.x - editor->saveas_b.area.upLeft.x;
        f32 fX     = (f32)(engine->backbuffer.width);
        f32 fY     = (f32)(engine->backbuffer.height);

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetDstX = (u32)(editor->saveas_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->saveas_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM)
        {
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
            engine->controls.keymap    &= ~ME_BIT_ENTER;
            meChangeState(editor, ME_STATE_SAVEAS);
        }
    }
    else if(!editor->i_button)
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_DEFAULT]);

        if(engine->controls.buttonmap & ME_BIT_LEFTM)
        {
            ME_DEBUG("clicked @ X: %f Y: %f",
                     engine->controls.pointer.x, engine->controls.pointer.y);
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
        }
    }
}

f_internal void getSheetDims
(
    EngineData *engine,
    EditorData *editor,
    u32        *sheet_width,
    u32        *sheet_height
){
    *sheet_width  = engine->planes[ME_PLANE_TILESHEET].width;
    *sheet_height = engine->planes[ME_PLANE_TILESHEET].height;
    f32 max_w = (f32)engine->backbuffer.width  * 0.8f - 2 * editor->tilesize;
    f32 max_h = (f32)engine->backbuffer.height * 0.8f - 3 * editor->tilesize;
    if((f32)*sheet_width > max_w)
    {
        *sheet_width = (u32)max_w;
    }
    if((f32)*sheet_height > max_h)
    {
        *sheet_height = (u32)max_h;
    }
}

f_internal void drawTilePicker
(
    EngineData *engine,
    EditorData *editor,
    u32   sheet_width,
    u32   sheet_height
){
    rvCompositeSettings comp = {0};
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.src        = &engine->planes[ME_PLANE_BACKGROUND];
    comp.offsetDstX = engine->backbuffer.width  / 10;
    comp.offsetDstY = engine->backbuffer.height / 10;
    comp.cropWidth  = (u32)((f32)engine->backbuffer.width  / 1.25f);
    comp.cropHeight = (u32)((f32)engine->backbuffer.height / 1.25f);

    rvCompositeImage(engine, &comp);

    comp.src        = &engine->planes[ME_PLANE_TILEPICKER];
    comp.offsetDstX = 0;
    comp.offsetDstY = 0;
    comp.cropWidth  = engine->planes[ME_PLANE_TILEPICKER].width;
    comp.cropHeight = engine->planes[ME_PLANE_TILEPICKER].height;

    rvCompositeImage(engine, &comp);

    comp.src        = &engine->planes[ME_PLANE_TILESHEET];
    comp.offsetSrcX = 0;
    comp.offsetSrcY = editor->viewScroll * editor->tilesize;
    comp.offsetDstX = engine->backbuffer.width  / 10 + editor->tilesize;
    comp.offsetDstY = engine->backbuffer.height / 10 + editor->tilesize;
    comp.cropWidth  = sheet_width;
    comp.cropHeight = sheet_height;

    rvCompositeImage(engine, &comp);
}

f_internal void drawContextMenu
(
    EngineData *engine,
    EditorData *editor,
    u32   tileLocX,
    u32   tileLocY
){
    u32 scrollHeight = editor->viewScroll * editor->tilesize;

    rvCompositeSettings comp = {0};
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT_SOLID];
    comp.offsetDstX = tileLocX + (u32)(editor->tilesize * 0.60f);
    comp.offsetDstY = tileLocY + (u32)(editor->tilesize * 0.60f) - scrollHeight;
    comp.cropWidth  = engine->planes[ME_PLANE_CONTEXTMENU].width;
    comp.cropHeight = engine->planes[ME_PLANE_CONTEXTMENU].height;
    rvCompositeImage(engine, &comp);

    f32 fX = (f32)engine->backbuffer.width;
    f32 fY = (f32)engine->backbuffer.height;

    f32    fPad    = 0.003f;
    u32 padding = (u32)(fX * fPad);

    f32 fOffsetX = (f32)comp.offsetDstX / fX;
    f32 fOffsetY = (f32)comp.offsetDstY / fY;

    f32 collision_length = (f32)editor->collision_b.name.size * (17.0f) / fX + fPad;
    editor->collision_b.area.upLeft.x   = fOffsetX + fPad;
    editor->collision_b.area.upLeft.y   = fOffsetY;
    editor->collision_b.area.lowRight.x = fOffsetX + collision_length;
    editor->collision_b.area.lowRight.y = fOffsetY + 16.0f / fY + fPad / 2;

    f32 animation_length = (f32)editor->animation_b.name.size * 17.0f / fX + fPad;
    editor->animation_b.area.upLeft.x   = fOffsetX + fPad;
    editor->animation_b.area.upLeft.y   = fOffsetY + 16.0f / fY + fPad;
    editor->animation_b.area.lowRight.x = fOffsetX + animation_length;
    editor->animation_b.area.lowRight.y = fOffsetY + 32.0f / fY + fPad * 1.5f;

    comp.src         = &engine->planes[ME_PLANE_CONTEXTMENU];
    comp.offsetDstX += padding;
    comp.offsetDstY += padding;
    rvCompositeImage(engine, &comp);
}

f_internal void getTileXY
(
    EngineData *engine,
    EditorData *editor,
    u16        *tileX,
    u16        *tileY,
    f32        fX,
    f32        fY
){
    *tileX = (u16)(engine->controls.pointer.x * fX / editor->tilesize);
    *tileY = (u16)(engine->controls.pointer.y * fY / editor->tilesize);

    u08 modX = *tileX % editor->selectMult;
    u08 modY = *tileY % editor->selectMult;
    if(engine->controls.keymap & ME_BIT_LSHIFT)
    {
        *tileX -= modX;
        *tileY -= modY;
    }
}

void meDrawEditor
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[ME_PLANE_VOID];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.cropWidth  = engine->planes[ME_PLANE_VOID].width;
    comp.cropHeight = engine->planes[ME_PLANE_VOID].height;

    rvCompositeImage(engine, &comp);

    for(u08 i = 0; i < editor->mapLayers; ++i)
    {
        if(engine->controls.keymap & (ME_BIT_LAYER0 << i))
        {
            editor->currentLayer = i;
            editor->isolate      = engine->controls.keymap & ME_BIT_LSHIFT;
        }
    }

    u08 startLayer = editor->isolate ? editor->currentLayer : 0;
    u08 endLayer   = editor->isolate ? editor->currentLayer + 1 : editor->mapLayers;

    comp.cropWidth  = editor->tilesize;
    comp.cropHeight = editor->tilesize;

    for(u08 z = startLayer; z < endLayer; ++z)
    {
        for(u32 y = 0; y < editor->mapHeight; ++y)
        {
            for(u32 x = 0; x < editor->mapWidth; ++x)
            {
                u64 index = z * editor->mapWidth * editor->mapHeight +
                            y * editor->mapWidth + x;

                if(editor->placedTiles[index].x == U16MAX)
                {
                    continue;
                }

                comp.src        = &engine->planes[ME_PLANE_TILESHEET];
                comp.offsetSrcX = editor->placedTiles[index].x * editor->tilesize;
                comp.offsetSrcY = editor->placedTiles[index].y * editor->tilesize;
                comp.offsetDstX = x * editor->tilesize;
                comp.offsetDstY = y * editor->tilesize;

                rvCompositeImage(engine, &comp);

                if(!(editor->flags & ME_FLAG_WIREFRAME))
                {
                    continue;
                }

                u64 sheetW = engine->planes[ME_PLANE_TILESHEET].width /
                             editor->tilesize;

                u64 dataIndex = editor->placedTiles[index].y * sheetW +
                                editor->placedTiles[index].x;

                if(editor->tileData[dataIndex].flags & RV_TILE_BIT_COLLISION)
                {
                    comp.src = &engine->planes[ME_PLANE_COLLISION];
                    rvCompositeImage(engine, &comp);
                }
            }
        }
    }

    char layerStr[2];
    snprintf(layerStr, 2, "%hhu", editor->currentLayer);

    StringView layer_sv = pdCstrSV(layerStr);

    rvLoadTextSettings set = {0};
    set.sv       = &layer_sv;
    set.image    = &engine->planes[ME_PLANE_CURRENTLAYER];
    set.font     = ME_PLANE_FONT16;
    set.charsize = 16;
    set.spacing  = 1;

    rvLoadText(engine, &set);

    comp.src        = &engine->planes[ME_PLANE_CURRENTLAYER];
    comp.offsetSrcX = 0;
    comp.offsetSrcY = 0;
    comp.offsetDstX = (u32)(0.95f * (f32)engine->backbuffer.width);
    comp.offsetDstY = (u32)(0.025f * (f32)engine->backbuffer.width);
    comp.cropWidth  = engine->planes[ME_PLANE_CURRENTLAYER].width;
    comp.cropHeight = engine->planes[ME_PLANE_CURRENTLAYER].height;

    rvCompositeImage(engine, &comp);

    f32 deltaMS = rvDeltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        comp.src        = &engine->planes[ME_PLANE_ICON_SAVED];
        comp.cropWidth  = engine->planes[ME_PLANE_ICON_SAVED].width;
        comp.cropHeight = engine->planes[ME_PLANE_ICON_SAVED].height;
        comp.offsetDstX = 16;
        comp.offsetDstY = 16;

        rvCompositeImage(engine, &comp);
    }

    f32 fX = (f32)engine->backbuffer.width;
    f32 fY = (f32)engine->backbuffer.height;

    u16 tileX = 0;
    u16 tileY = 0;
    getTileXY(engine, editor, &tileX, &tileY, fX, fY);

    comp.src        = &engine->planes[ME_PLANE_TILESHEET];
    comp.dst        = &engine->backbuffer;
    comp.offsetSrcX = editor->selectedX * editor->tilesize;
    comp.offsetSrcY = editor->selectedY * editor->tilesize;
    comp.offsetDstX = tileX * editor->tilesize;
    comp.offsetDstY = tileY * editor->tilesize;
    comp.cropWidth  = editor->tilesize  * editor->selectMult;
    comp.cropHeight = editor->tilesize  * editor->selectMult;

    rvCompositeImage(engine, &comp);

    u32 sheet_width  = 0;
    u32 sheet_height = 0;
    getSheetDims(engine, editor, &sheet_width, &sheet_height);

    if(editor->flags & ME_FLAG_TILEPICKER)
    {
        drawTilePicker(engine, editor, sheet_width, sheet_height);
        if(editor->flags & ME_FLAG_CONTEXTMENU)
        {
            f32 tileCornerX = 0.095f + (f32)((f32)editor->tilesize / fX);
            f32 tileCornerY = 0.095f + (f32)((f32)editor->tilesize / fY);
            u32 tileLocX = (u32)(fX * (tileCornerX + 0.0055f) +
                                           editor->selectedX * editor->tilesize);
            u32 tileLocY = (u32)(fY * (tileCornerY + 0.006f) +
                                           editor->selectedY * editor->tilesize);
            drawContextMenu(engine, editor, tileLocX, tileLocY);
        }
    }

    if(engine->config.choices & ME_CHOICE_SHOW_FPS_BIT)
    {
        updateFPS(engine, editor);

        comp.src        = &engine->planes[ME_PLANE_FPS];
        comp.cropWidth  = engine->planes[ME_PLANE_FPS].width;
        comp.cropHeight = engine->planes[ME_PLANE_FPS].height;
        comp.offsetSrcX = 0;
        comp.offsetSrcY = 0;
        comp.offsetDstX = 0;
        comp.offsetDstY = 0;

        rvCompositeImage(engine, &comp);
    }
}

f_internal u32 sheetIndex
(
    EngineData *engine,
    EditorData *editor
){
    u32 tileW = engine->planes[ME_PLANE_TILESHEET].width /
                     editor->tilesize;
    return editor->selectedY * tileW + editor->selectedX;
}

f_internal void pollContextMenu
(
    EngineData *engine,
    EditorData *editor
){
    if(engine->controls.buttonmap & ME_BIT_RIGHTM ||
       engine->controls.keymap    & ME_BIT_MENU
    ){
        editor->flags              &= ~ME_FLAG_CONTEXTMENU;
        engine->controls.keymap    &= ~ME_BIT_MENU;
        engine->controls.buttonmap &= ~ME_BIT_RIGHTM;
        return;
    }

    // to make room for key controls later
    if(rvInsideRect(&engine->controls.pointer, &editor->collision_b.area))
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);
        editor->collision_b.status |= RV_BIT_HOVER;
        editor->animation_b.status &= ~RV_BIT_HOVER;
    }
    else if(rvInsideRect(&engine->controls.pointer, &editor->animation_b.area))
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);
        editor->collision_b.status &= ~RV_BIT_HOVER;
        editor->animation_b.status |= RV_BIT_HOVER;
    }
    else
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_DEFAULT]);
        editor->collision_b.status &= ~RV_BIT_HOVER;
        editor->animation_b.status &= ~RV_BIT_HOVER;
    }

    f32 fX = (f32)(engine->backbuffer.width);
    f32 fY = (f32)(engine->backbuffer.height);

    bool confirmed = engine->controls.buttonmap & ME_BIT_LEFTM ||
                     engine->controls.keymap    & ME_BIT_ENTER;

    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RV_PICTOP_OVER;
    comp.src    = &engine->planes[ME_PLANE_BACKGROUND];

    if(editor->collision_b.status & RV_BIT_HOVER)
    {
        f32 length = editor->collision_b.area.lowRight.x -
                       editor->collision_b.area.upLeft.x;

        comp.offsetDstX = (u32)(editor->collision_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->collision_b.area.upLeft.y * fY) + 20;
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;
        rvCompositeImage(engine, &comp);

        if(confirmed)
        {
            u32 index = sheetIndex(engine, editor);
            editor->tileData[index].flags ^= RV_TILE_BIT_COLLISION;

            editor->flags &= ~ME_FLAG_CONTEXTMENU;
            engine->controls.keymap    &= ~ME_BIT_MENU;
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
            return;
        }
    }
    else if(editor->animation_b.status & RV_BIT_HOVER)
    {
        f32 length = editor->animation_b.area.lowRight.x -
                       editor->animation_b.area.upLeft.x;

        comp.offsetDstX = (u32)(editor->animation_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->animation_b.area.upLeft.y * fY) + 20;
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;
        rvCompositeImage(engine, &comp);

        if(confirmed)
        {
            u32 index = sheetIndex(engine, editor);
            editor->tileData[index].flags ^= RV_TILE_BIT_ANIMATED;

            ME_WARN("TODO: edit tile animation: (%u,%u)",
                    editor->selectedX, editor->selectedY);

            editor->flags &= ~ME_FLAG_CONTEXTMENU;
            engine->controls.keymap    &= ~ME_BIT_MENU;
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
            return;
        }
    }
}

f_internal void pollTilePicker
(
    EngineData *engine,
    EditorData *editor,
    u32        sheet_width,
    u32        sheet_height
){
    f32 fX = (f32)(engine->backbuffer.width);
    f32 fY = (f32)(engine->backbuffer.height);

    Rect tiles = {0};
    tiles.upLeft.x   = 0.095f + (f32)((f32)editor->tilesize / fX);
    tiles.upLeft.y   = 0.095f + (f32)((f32)editor->tilesize / fY);
    tiles.lowRight.x = tiles.upLeft.x + (f32)sheet_width  / fX;
    tiles.lowRight.y = tiles.upLeft.y + (f32)sheet_height / fY;

    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RV_PICTOP_OVER;

    if(engine->controls.keymap & ME_BIT_TILEPICKER ||
       engine->controls.keymap & ME_BIT_MENU
    ){
        editor->flags           &= ~ME_FLAG_TILEPICKER;
        editor->flags           &= ~ME_FLAG_CONTEXTMENU;
        engine->controls.keymap &= ~ME_BIT_MENU;
        engine->controls.keymap &= ~ME_BIT_TILEPICKER;
        return;
    }
    else if(engine->controls.keymap & ME_BIT_TILEPICKER)
    {
        editor->flags           |= ME_FLAG_TILEPICKER;
        engine->controls.keymap &= ~ME_BIT_TILEPICKER;
    }

    if(engine->controls.keymap & ME_BIT_DECREASE)
    {
        meUpdateSelectSize(editor, false);
        engine->controls.keymap &= ~ME_BIT_DECREASE;
    }
    if(engine->controls.keymap & ME_BIT_INCREASE)
    {
        meUpdateSelectSize(editor, true);
        engine->controls.keymap &= ~ME_BIT_INCREASE;
    }

    if(rvInsideRect(&engine->controls.pointer, &editor->close_b.area))
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);

        f32 length = editor->close_b.area.lowRight.x -
                       editor->close_b.area.upLeft.x;

        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.offsetSrcX = 0;
        comp.offsetSrcY = 0;
        comp.offsetDstX = (u32)(editor->close_b.area.upLeft.x * fX);
        comp.offsetDstY = (u32)(editor->close_b.area.upLeft.y * fY + 20);
        comp.cropWidth  = (u32)(length * fX);
        comp.cropHeight = 5;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_LEFTM)
        {
            editor->flags              &= ~ME_FLAG_TILEPICKER;
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
        }
    }
    else if(rvInsideRect(&engine->controls.pointer, &tiles))
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_NULL]);

        f32 deltaX = engine->controls.pointer.x - tiles.upLeft.x;
        f32 deltaY = engine->controls.pointer.y - tiles.upLeft.y;
        u16 tileX  = (u16)(deltaX * fX / editor->tilesize);
        u16 tileY  = (u16)(deltaY * fY / editor->tilesize);
        u16 sheetX = (u16)(sheet_width  / editor->tilesize);
        u16 sheetY = (u16)(sheet_height / editor->tilesize);

        if(tileX + editor->selectMult > sheetX)
        {
            tileX = (u08)(sheetX - editor->selectMult);
        }
        if(tileY + editor->selectMult > sheetY)
        {
            tileY = (u08)(sheetY - editor->selectMult);
        }

        u32 tileLocX = (u32)(fX * (tiles.upLeft.x + 0.0055f) +
                             tileX * editor->tilesize);
        u32 tileLocY = (u32)(fY * (tiles.upLeft.y + 0.006f) +
                             tileY * editor->tilesize);

        comp.offsetDstX = tileLocX;
        comp.offsetDstY = tileLocY;
        comp.src        = &engine->planes[ME_PLANE_HIGHLIGHT];
        comp.cropWidth  = editor->tilesize * editor->selectMult;
        comp.cropHeight = editor->tilesize * editor->selectMult;

        rvCompositeImage(engine, &comp);

        if(engine->controls.buttonmap & ME_BIT_RIGHTM)
        {
            editor->selectedX           =  tileX;
            editor->selectedY           =  tileY + editor->viewScroll;
            editor->flags              |= ME_FLAG_CONTEXTMENU;
            engine->controls.buttonmap &= ~ME_BIT_RIGHTM;
        }
        else if(engine->controls.buttonmap & ME_BIT_LEFTM)
        {
            editor->selectedX           = tileX;
            editor->selectedY           = tileY + editor->viewScroll;
            editor->flags              &= ~ME_FLAG_TILEPICKER;
            engine->controls.buttonmap &= ~ME_BIT_LEFTM;
        }
    }
    else
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_DEFAULT]);
    }
}

void mePollEditor
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.dst    = &engine->backbuffer;
    comp.pictop = RV_PICTOP_OVER;

    f32 fX = (f32)engine->backbuffer.width;
    f32 fY = (f32)engine->backbuffer.height;

    u32 sheet_width  = 0;
    u32 sheet_height = 0;
    getSheetDims(engine, editor, &sheet_width, &sheet_height);

    bool control = engine->controls.keymap & ME_BIT_LCTRL ||
                   engine->controls.keymap & ME_BIT_RCTRL;

    if(control && engine->controls.keymap & ME_BIT_WIREFRAME)
    {
        editor->flags           ^= ME_FLAG_WIREFRAME;
        engine->controls.keymap &= ~ME_BIT_WIREFRAME;
    }

    if(editor->flags & ME_FLAG_TILEPICKER)
    {
        if(editor->flags & ME_FLAG_CONTEXTMENU)
        {
            pollContextMenu(engine, editor);
        }
        else
        {
            pollTilePicker(engine, editor, sheet_width, sheet_height);
        }
        return;
    }

    if(engine->controls.keymap & ME_BIT_TILEPICKER)
    {
        editor->flags           |= ME_FLAG_TILEPICKER;
        engine->controls.keymap &= ~ME_BIT_TILEPICKER;
    }

    if(control &&engine->controls.keymap & ME_BIT_UNDO)
    {
        meUndo(editor);
        engine->controls.keymap &= ~ME_BIT_UNDO;
        return;
    }
    else if(control && engine->controls.keymap & ME_BIT_REDO)
    {
        meRedo(editor);
        engine->controls.keymap &= ~ME_BIT_REDO;
        return;
    }
    else if(engine->controls.keymap & ME_BIT_MENU)
    {
        meChangeState(editor, ME_STATE_MENU);
        engine->controls.keymap &= ~ME_BIT_MENU;
        return;
    }
    else if((engine->controls.keymap & ME_BIT_LSHIFT  ||
             engine->controls.keymap & ME_BIT_RSHIFT) &&
             engine->controls.keymap & ME_BIT_LCTRL   &&
             engine->controls.keymap & ME_BIT_SAVE
    ){
        engine->controls.ascii = 0x00;
        meChangeState(editor, ME_STATE_SAVEAS);
    }
    else if(engine->controls.keymap & ME_BIT_LCTRL &&
            engine->controls.keymap & ME_BIT_SAVE
    ){
        meSaveProject(engine, editor);
        engine->controls.keymap &= ~ME_BIT_SAVE;
        return;
    }

    u16 tileX = 0;
    u16 tileY = 0;
    getTileXY(engine, editor, &tileX, &tileY, fX, fY);

    //if(rvInsideRect(&engine->controls.pointer, &editor->button))
    // {
    //     rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        rvChangeCursor(engine, &engine->planes[ME_PLANE_CURSOR_PLACE]);

        if(engine->controls.buttonmap & ME_BIT_LEFTM)
        {
            mePlaceSelectedTiles(engine, editor, tileX, tileY);
        }
    }
}

void meDrawFilePicker
(
    EngineData *engine
){
    rvCompositeSettings comp = {0};
    comp.src        = &engine->planes[ME_PLANE_BACKGROUND];
    comp.dst        = &engine->backbuffer;
    comp.pictop     = RV_PICTOP_OVER;
    comp.cropWidth  = engine->planes[ME_PLANE_BACKGROUND].width;
    comp.cropHeight = engine->planes[ME_PLANE_BACKGROUND].height;

    rvCompositeImage(engine, &comp);

    comp.src = &engine->planes[ME_PLANE_CURRENTFILE];
    comp.offsetDstX = (u32)(0.025f * (f32)engine->backbuffer.width);
    comp.offsetDstY = (u32)(0.4f * (f32)engine->backbuffer.height);
    comp.cropWidth  = engine->planes[ME_PLANE_CURRENTFILE].width;
    comp.cropHeight = engine->planes[ME_PLANE_CURRENTFILE].height;

    rvCompositeImage(engine, &comp);
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
    editor->inputBuffer.data[editor->cursor] = RV_ASCII_CURSOR;

    if(engine->controls.keymap & ME_BIT_MENU)
    {
        meChangeState(editor, ME_STATE_MENU);
        engine->controls.keymap &= ~ME_BIT_MENU;
        return;
    }

    if(engine->controls.ascii)
    {
        editor->inputBuffer.data[editor->cursor] = engine->controls.ascii;
        engine->controls.ascii = 0x00;

        increaseCursor(editor);
    }

    if(engine->controls.keymap & ME_BIT_BACKSPACE)
    {
        editor->inputBuffer.data[editor->cursor] = '\0';

        decreaseCursor(editor);

        engine->controls.keymap &= ~ME_BIT_BACKSPACE;
    }

    if(engine->controls.keymap & ME_BIT_ENTER)
    {
        editor->inputBuffer.data[editor->cursor] = '\0';

        editor->confirmed = true;
        engine->controls.keymap &= ~ME_BIT_ENTER;
    }
}

f_internal void drawTextBuffer
(
    EngineData *engine,
    EditorData *editor
){
    rvLoadTextSettings set = {0};
    set.image    = &engine->planes[ME_PLANE_CURRENTFILE];
    set.sv       = (StringView*)&editor->inputBuffer;
    set.font     = ME_PLANE_FONT16,
    set.charsize = 16;
    set.spacing  = 1;

    rvLoadText(engine, &set);
}

void mePollLoadFile
(
    EngineData *engine,
    EditorData *editor
){
    pollInput(engine, editor);

    if(editor->confirmed)
    {
        meLoadProject(engine, editor);
        meChangeState(editor, ME_STATE_EDIT);
        editor->confirmed = false;
        return;
    }

    drawTextBuffer(engine, editor);
}

void mePollSaveFile
(
    EngineData *engine,
    EditorData *editor
){
    pollInput(engine, editor);

    if(editor->confirmed)
    {
        meSaveProject(engine, editor);
        meChangeState(editor, ME_STATE_EDIT);
        editor->confirmed = false;
        return;
    }

    drawTextBuffer(engine, editor);
}
