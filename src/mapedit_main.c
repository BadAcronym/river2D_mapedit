#include "mapedit_main.h"
#include "imgsurf_main.h"

#include "string_view.h"

#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

// BACKLOG: write logging function (or library, lmfao) that writes to file, as
// well as stderr copy some river3D code for that?

void mapedit_init
(
    EngineData *engine,
    EditorData *editor
){
    river2D_loadImage_file(engine, "assets/background.qoi",
                           &engine->planes[MAPEDIT_PLANE_BACKGROUND],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_BACKGROUND].data)
    {
        fprintf(stderr,
                "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/black.qoi",
                           &engine->planes[MAPEDIT_PLANE_VOID],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_VOID].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load black image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/highlight.qoi",
                           &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_HIGHLIGHT].data)
    {
        fprintf(stderr,
                "\n\033[31;1;7mERROR: Unable to load highlight image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/saving.qoi",
                           &engine->planes[MAPEDIT_PLANE_ICON_SAVING],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVING].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saving icon!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/saved.qoi",
                           &engine->planes[MAPEDIT_PLANE_ICON_SAVED],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVED].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saved icon!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/cursor_default.qoi",
                           &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load default cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/cursor_hover.qoi",
                           &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_HOVER].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load hover cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/cursor_place.qoi",
                           &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_PLACE].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load place cursor!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/font_default_16.qoi",
                           &engine->planes[MAPEDIT_PLANE_FONT16],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_FONT16].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load font image!\033[0m\n");
    }

    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_NULL], 32, 32);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_CURRENTFILE], 32, 32);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],
                        engine->backbuffer.width, engine->backbuffer.height);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU],
                        engine->backbuffer.width, engine->backbuffer.height);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE],
                        engine->backbuffer.width, engine->backbuffer.height);

    // BACKLOG: transfer to mapedit_loadConfig at some point, when keybinds
    // should be remappable?

    engine->controls.buttoncodes[MAPEDIT_BUTTON_LEFTM]   = RIVER2D_MOUSE1;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_MIDDLEM] = RIVER2D_MOUSE2;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_RIGHTM]  = RIVER2D_MOUSE3;

    engine->controls.keycodes[MAPEDIT_KEY_ESCAPE]     = RIVER2D_ASCII_ESCAPE;
    engine->controls.keycodes[MAPEDIT_KEY_TAB]        = RIVER2D_ASCII_TAB;
    engine->controls.keycodes[MAPEDIT_KEY_ENTER]      = RIVER2D_ASCII_ENTER;
    engine->controls.keycodes[MAPEDIT_KEY_LSHIFT]     = RIVER2D_ASCII_LSHIFT;
    engine->controls.keycodes[MAPEDIT_KEY_RSHIFT]     = RIVER2D_ASCII_RSHIFT;
    engine->controls.keycodes[MAPEDIT_KEY_LCTRL]      = RIVER2D_ASCII_LCTRL;
    engine->controls.keycodes[MAPEDIT_KEY_RCTRL]      = RIVER2D_ASCII_RCTRL;
    engine->controls.keycodes[MAPEDIT_KEY_BACKSPACE]  = RIVER2D_ASCII_BACKSPACE;
    engine->controls.keycodes[MAPEDIT_KEY_SPACE]      = RIVER2D_ASCII_SPACE;
    engine->controls.keycodes[MAPEDIT_KEY_0]          = '0';
    engine->controls.keycodes[MAPEDIT_KEY_1]          = '1';
    engine->controls.keycodes[MAPEDIT_KEY_2]          = '2';
    engine->controls.keycodes[MAPEDIT_KEY_3]          = '3';
    engine->controls.keycodes[MAPEDIT_KEY_4]          = '4';
    engine->controls.keycodes[MAPEDIT_KEY_5]          = '5';
    engine->controls.keycodes[MAPEDIT_KEY_6]          = '6';
    engine->controls.keycodes[MAPEDIT_KEY_7]          = '7';
    engine->controls.keycodes[MAPEDIT_KEY_8]          = '8';
    engine->controls.keycodes[MAPEDIT_KEY_9]          = '9';
    engine->controls.keycodes[MAPEDIT_KEY_A]          = 'a';
    engine->controls.keycodes[MAPEDIT_KEY_B]          = 'b';
    engine->controls.keycodes[MAPEDIT_KEY_C]          = 'c';
    engine->controls.keycodes[MAPEDIT_KEY_D]          = 'd';
    engine->controls.keycodes[MAPEDIT_KEY_E]          = 'e';
    engine->controls.keycodes[MAPEDIT_KEY_F]          = 'f';
    engine->controls.keycodes[MAPEDIT_KEY_G]          = 'g';
    engine->controls.keycodes[MAPEDIT_KEY_H]          = 'h';
    engine->controls.keycodes[MAPEDIT_KEY_I]          = 'i';
    engine->controls.keycodes[MAPEDIT_KEY_J]          = 'j';
    engine->controls.keycodes[MAPEDIT_KEY_K]          = 'k';
    engine->controls.keycodes[MAPEDIT_KEY_L]          = 'l';
    engine->controls.keycodes[MAPEDIT_KEY_M]          = 'm';
    engine->controls.keycodes[MAPEDIT_KEY_N]          = 'n';
    engine->controls.keycodes[MAPEDIT_KEY_O]          = 'o';
    engine->controls.keycodes[MAPEDIT_KEY_P]          = 'p';
    engine->controls.keycodes[MAPEDIT_KEY_Q]          = 'q';
    engine->controls.keycodes[MAPEDIT_KEY_R]          = 'r';
    engine->controls.keycodes[MAPEDIT_KEY_S]          = 's';
    engine->controls.keycodes[MAPEDIT_KEY_T]          = 't';
    engine->controls.keycodes[MAPEDIT_KEY_U]          = 'u';
    engine->controls.keycodes[MAPEDIT_KEY_V]          = 'v';
    engine->controls.keycodes[MAPEDIT_KEY_W]          = 'w';
    engine->controls.keycodes[MAPEDIT_KEY_X]          = 'x';
    engine->controls.keycodes[MAPEDIT_KEY_Y]          = 'y';
    engine->controls.keycodes[MAPEDIT_KEY_Z]          = 'z';
    engine->controls.keycodes[MAPEDIT_KEY_MINUS]      = '-';
    engine->controls.keycodes[MAPEDIT_KEY_EQUAL]      = '=';

    StringView title_sv = puddle_cstr_sv("RIVER2D MAP EDITOR");
    StringView new_sv   = puddle_cstr_sv("NEW PROJECT");
    StringView load_sv  = puddle_cstr_sv("LOAD PROJECT");
    StringView save_sv  = puddle_cstr_sv("SAVE PROJECT");
    StringView quit_sv  = puddle_cstr_sv("QUIT");
    StringView close    = puddle_cstr_sv("CLOSE");

    // JANKY: I'm loading text by creating a button, then overwriting it. pause/main
    Coordinates point = { .x = 0.5f, .y = 0.2f };

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU], &title_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->new_b);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], &title_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->new_b);
    point.y = 0.4f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU], &new_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->new_b);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], &new_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->new_b);

    point.y = 0.5f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU], &load_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->load_b);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], &load_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->load_b);
    point.y = 0.6f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], &save_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->save_b);
    point.y = 0.8f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU], &quit_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->quit_b);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], &quit_sv,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->quit_b);

    point.x = 0.175f;
    point.y = 0.86f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], &close,
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->close_b);

    // HACK: load upfront for now
    // TODO: load all .qoi files from folder, then append to one big tilesheet
    // in memory
    // BACKLOG: allow replacing this tilesheet by selecting one or
    // multiple files with a file browser...
    river2D_loadImage_file(engine, "assets/tiles/tilesheet.qoi",
                           &engine->planes[MAPEDIT_PLANE_TILESHEET],
                           RIVER2D_CHANNELS_BGRA, 8);

    // BACKLOG: allow decreasing/increasing this minimum tilesize, as well as limiting
    // selectmult?
    editor->tilesize   = 8;
    editor->selectMult = 1;

    // allow 10 layers by default, any other layer you'd have to add to the UI selector
    // PERF: start with 1 layer, only poll and draw that layer, until
    // there's any data written to the other layers 🤔 means to keep track of
    // what layer has had data written to it, and only go through those??
    editor->layers       = 10;
    editor->currentLayer = 1;

    editor->mapWidth  = engine->config.canvas_width  / editor->tilesize;
    editor->mapHeight = engine->config.canvas_height / editor->tilesize;

    uint64_t tilecount = editor->layers * editor->mapWidth * editor->mapHeight;
    // TODO: resize this when user decides to expand canvas, set map_width & _height
    editor->tiles = malloc(tilecount * sizeof(Tile));
    for(uint32_t i = 0; i < tilecount; ++i)
    {
        editor->tiles[i].x = UINT16_MAX;
        editor->tiles[i].y = UINT16_MAX;
    }

    editor->actions = malloc(MAPEDIT_MAX_ACTIONS * sizeof(Action));
    for(uint32_t i = 0; i < MAPEDIT_MAX_ACTIONS; ++i)
    {
        editor->actions[i].stroke_start.s  = INT64_MIN;
        editor->actions[i].stroke_start.ns = INT64_MIN;
    }

    if(!editor->projectName.data)
    {
        editor->projectName = puddle_cstr_s("unnamed_project");
    }

    River2D_Time now                = river2D_queryTime();
    editor->lastPresentTime         = now;
    editor->lastSaveTime.s          = 1;
    editor->lastSaveTime.ns         = 1;
    engine->controls.lastScrollTime = now;

    editor->currentState = MAPEDIT_STATE_MENU;
    char *buf = calloc(256, 1);
    editor->filename.data = buf;
    editor->filename.size = 256;
}

int32_t mapedit_shutdown
(
    EditorData *editor
){
    free(editor->tiles);
    free(editor->actions);
    free((void*)editor->filename.data);
    return 0;
}

f_internal void changeState
(
    EditorData *editor,
    uint8_t    nextState
){
    if(editor->currentState == nextState)
    {
        fprintf(stderr, "\n\033[33;1;7mWARNING: trying to change state to the same "
        "state: %u.\033[0m\n", editor->currentState);
        return;
    }

    editor->previousState = editor->currentState;
    editor->currentState  = nextState;
}

// TODO: add RLE? lose 1 bit of the 32 for every tile, just to make some
// uint32_t's a repeat of the last placed tile see if this is even worth it,
// it'd only be in exact runs find another way to do RLE maybe, one that scales
// well with lots of pairs of 2, 4, short runs that is

f_internal void saveCurrentProject
(
    EngineData *engine,
    EditorData *editor
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVING],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                           engine->planes[MAPEDIT_PLANE_ICON_SAVING].width,
                           engine->planes[MAPEDIT_PLANE_ICON_SAVING].height);

    engine->bltBuffer(engine);

    char *filename_cstr = malloc(256);
    sprintf(filename_cstr, "%s.rte", editor->projectName.data);

    FILE *file = fopen(filename_cstr, "wb");
    if(!file)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: could not open file for saving: "
                "%s.\033[0m\n", editor->projectName.data);
        free(filename_cstr);
        return;
    }

    const char header[9] = "r2Dtiles";
    fwrite(header, sizeof(header) - 1, 1, file);

    size_t elements = 0;

    if((elements = fwrite(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. "
                "\033[0m\n");
        return;
    }

    uint64_t tilecount = editor->layers * editor->mapHeight * editor->mapWidth;
    fwrite(editor->tiles, sizeof(Tile), tilecount, file);

    imgsurf_write_ptr(file, engine->planes[MAPEDIT_PLANE_TILESHEET].data,
                      IMGSURF_FILE_QOI,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].height,
                      IMGSURF_CHANNELS_BGRA, 8);

    fclose(file);

    free(filename_cstr);
    fprintf(stdout, "Project saved successfully.\n");

    editor->lastSaveTime = river2D_queryTime();
}

f_internal void drawMainMenu
(
    EngineData *engine,
    EditorData *editor
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].width,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].height);

    if(!editor->previousState)
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                               engine->planes[MAPEDIT_PLANE_MAINMENU].width,
                               engine->planes[MAPEDIT_PLANE_MAINMENU].height);
    }
    else
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                               engine->planes[MAPEDIT_PLANE_PAUSEMENU].width,
                               engine->planes[MAPEDIT_PLANE_PAUSEMENU].height);
    }

    // BACKLOG: make these icons use some sort of opacity, fade-out animation
    // (and / or animation in general)...
    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVED],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].width,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].height);
    }
}

f_internal void checkMainMenuButtons
(
    EngineData *engine,
    EditorData *editor
){
    if(engine->controls.keymap & MAPEDIT_BIT_LCTRL &&
       engine->controls.keymap & MAPEDIT_BIT_Q
    ){
        engine->running = false;
        return;
    }

    if(engine->controls.keymap &  MAPEDIT_BIT_ESCAPE &&
       editor->previousState   != MAPEDIT_STATE_NULL
    ){
        changeState(editor, editor->previousState);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    if(river2D_insideRect(&engine->controls.pointer, &editor->new_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->new_b.area.lowRight.x - editor->new_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->new_b.area.upLeft.x * fX),
                               (uint32_t)(editor->new_b.area.upLeft.y * fY + 20),
                               0, 0,
                               (uint32_t)(length * fX), 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            if(editor->previousState != MAPEDIT_STATE_NULL && editor->tiles)
            {
                uint64_t tilecount = editor->layers *
                                     editor->mapHeight * editor->mapWidth;
                for(uint32_t i = 0; i < tilecount; ++i)
                {
                    editor->tiles[i].x = UINT16_MAX;
                    editor->tiles[i].y = UINT16_MAX;
                }
            }

            changeState(editor, MAPEDIT_STATE_EDIT);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->load_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->load_b.area.lowRight.x - editor->load_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->load_b.area.upLeft.x * fX),
                               (uint32_t)(editor->load_b.area.upLeft.y * fY + 20),
                               0, 0,
                               (uint32_t)(length * fX), 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            changeState(editor, MAPEDIT_STATE_LOAD);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->quit_b.area))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->quit_b.area.lowRight.x - editor->quit_b.area.upLeft.x;
        float fX     = (float)(engine->backbuffer.width);
        float fY     = (float)(engine->backbuffer.height);
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->quit_b.area.upLeft.x * fX),
                               (uint32_t)(editor->quit_b.area.upLeft.y * fY + 20),
                               0, 0,
                               (uint32_t)(length * fX), 5);

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
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->save_b.area.upLeft.x * fX),
                               (uint32_t)(editor->save_b.area.upLeft.y * fY + 20),
                               0, 0, (uint32_t)(length * fX), 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            saveCurrentProject(engine, editor);
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

f_internal void drawEditor
(
    EngineData *engine,
    EditorData *editor
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_VOID],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);

    for(uint8_t i = 0; i < editor->layers; ++i)
    {
        if(engine->controls.keymap & (MAPEDIT_BIT_0 << i))
        {
            editor->currentLayer = i;

            if(engine->controls.keymap & MAPEDIT_BIT_LSHIFT)
            {
                editor->isolate = true;
            }
            else
            {
                editor->isolate = false;
            }
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
                if(editor->tiles[index].x != UINT16_MAX &&
                   editor->tiles[index].y != UINT16_MAX
                ){
                    engine->compositeImage(engine,
                                           &engine->planes[MAPEDIT_PLANE_TILESHEET],
                                           &engine->backbuffer, RIVER2D_PICTOP_OVER,
                                           x * editor->tilesize,
                                           y * editor->tilesize,
                                           editor->tiles[index].x * editor->tilesize,
                                           editor->tiles[index].y * editor->tilesize,
                                           editor->tilesize, editor->tilesize);
                }
            }
        }
    }

    char layerStr[2];
    snprintf(layerStr, 2, "%hhu", editor->currentLayer);

    StringView layer_sv = puddle_cstr_sv(layerStr);

    engine->loadText(engine, &engine->planes[MAPEDIT_PLANE_CURRENTLAYER], &layer_sv,
                     MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_CURRENTLAYER],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER,
                           (uint32_t)(0.95f * (float)engine->backbuffer.width),
                           (uint32_t)(0.025f * (float)engine->backbuffer.width),
                           0, 0,
                           engine->planes[MAPEDIT_PLANE_CURRENTLAYER].width,
                           engine->planes[MAPEDIT_PLANE_CURRENTLAYER].height);

    // BACKLOG: make these icons use some sort of opacity, fade-out animation
    // (and / or animation in general)...
    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVED],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].width,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].height);
    }
}

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

f_internal void undo
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

    if(prevAction.stroke_start.s == INT64_MIN &&
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

f_internal void redo
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

f_internal void placeSelectedTiles
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

            Tile new_tile = {editor->selectedX + x, editor->selectedY + y};

            writeAction(editor, index, new_tile);
            incrementAction(editor);
        }
    }
}

f_internal void checkEditorButtons
(
    EngineData *engine,
    EditorData *editor
){
    // TODO: (mapedit #6): make backbuffer moveable,
    // move grid along with it (so it might be offset)

    // TODO: (mapedit #2): add some descriptions or UI elements to delimit what
    // mode one is in

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
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10,
                               engine->backbuffer.height / 10,
                               0, 0,
                               (uint32_t)((float)engine->backbuffer.width  / 1.25f),
                               (uint32_t)((float)engine->backbuffer.height / 1.25f));

        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].width,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].height);

        // TODO: load each and every file that is in assets/tiles, then display
        // their thumbnails 🤔

        // HACK: just load the one file for now and use it here... lmfao
        uint32_t sheet_width  = engine->planes[MAPEDIT_PLANE_TILESHEET].width;
        uint32_t sheet_height = engine->planes[MAPEDIT_PLANE_TILESHEET].height;
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10 + editor->tilesize,
                               engine->backbuffer.height / 10 + editor->tilesize,
                               0, 0, sheet_width, sheet_height);

        Rect tiles = {0};
        tiles.upLeft.x  = 0.095f + (float)((float)editor->tilesize / fX);
        tiles.upLeft.y  = 0.095f + (float)((float)editor->tilesize / fY);
        tiles.lowRight.x = tiles.upLeft.x + (float)sheet_width / fX;
        tiles.lowRight.y = tiles.upLeft.y + (float)sheet_height / fY;

        // TODO: load all files inside the specified folder, separate the
        // spreadsheets visually but have them be one image

        // TODO: river2D_appendImage (either by x or y) would be super handy to
        // have in general

        // TODO: scrollwheel changes size, 2x2 smallest, 96x96 biggest? only
        // limitation is the highlighting, lol

        if(river2D_insideRect(&engine->controls.pointer, &editor->close_b.area))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            float length = editor->close_b.area.lowRight.x -
                           editor->close_b.area.upLeft.x;
            engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                                   &engine->backbuffer, RIVER2D_PICTOP_OVER,
                                   (uint32_t)(editor->close_b.area.upLeft.x * fX),
                                   (uint32_t)(editor->close_b.area.upLeft.y * fY + 20),
                                   0, 0, (uint32_t)(length * fX), 5);

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
            uint8_t  tileX  = (uint8_t)(deltaX * fX  / editor->tilesize);
            uint8_t  tileY  = (uint8_t)(deltaY * fY / editor->tilesize);
            uint16_t sheetX = (uint16_t)(sheet_width / editor->tilesize);
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

            engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                                   &engine->backbuffer, RIVER2D_PICTOP_OVER,
                                   (uint32_t)(fX * (tiles.upLeft.x + 0.0055f) +
                                              tileX * editor->tilesize),
                                   (uint32_t)(fY * (tiles.upLeft.y + 0.006f) +
                                              tileY * editor->tilesize),
                                   0, 0,
                                   editor->tilesize * editor->selectMult,
                                   editor->tilesize * editor->selectMult);

            if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
            {
                editor->selectedX           = tileX;
                editor->selectedY           = tileY;
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
        undo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_Z;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_Y &&
            engine->controls.keymap & MAPEDIT_BIT_LCTRL
    ){
        redo(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_Y;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LCTRL &&
            engine->controls.keymap & MAPEDIT_BIT_S
    ){
        saveCurrentProject(engine, editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_S;
        return;
    }

    // TODO: allow resizing the view to zoom in or out freely into the backbuffer.
    // TODO: allow scrolling and panning the view, as well as expanding the tilemap

    // BACKLOG: allow resizing the backbuffer itself to be smaller or bigger.
    // always keep a copy of the largest backbuffer in memory, so data is not lost when
    // sizing down, then back up?

    // TODO: wheel or menu of recently used tiles and a hotbar with specific
    // ones, somewhere.

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

    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER,
                           tileX * editor->tilesize,
                           tileY * editor->tilesize,
                           editor->selectedX * editor->tilesize,
                           editor->selectedY * editor->tilesize,
                           editor->tilesize  * editor->selectMult,
                           editor->tilesize  * editor->selectMult);

    //if(river2D_insideRect(&engine->controls.pointer, &editor->button))
    // {
    //     river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE]);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            placeSelectedTiles(editor, tileX, tileY);
        }
    }
}

f_internal void drawFilePicker
(
    EngineData *engine
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);

    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_CURRENTFILE],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER,
                           (uint32_t)(0.2f * (float)engine->backbuffer.width),
                           (uint32_t)(0.4f * (float)engine->backbuffer.height),
                           0, 0,
                           engine->planes[MAPEDIT_PLANE_CURRENTFILE].width,
                           engine->planes[MAPEDIT_PLANE_CURRENTFILE].height);
}

f_internal void loadProject
(
    EngineData *engine,
    EditorData *editor
){
    if(!editor->filename.data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to find .rte file to load!"
                "\033[0m\n");
        changeState(editor, MAPEDIT_STATE_MENU);
        return;
    }

#ifdef DEBUG
    fprintf(stderr, "\nloading file: "PRI_SV"\n", ARG_SV(*filename));
#endif

    FILE *file = fopen(editor->filename.data, "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file "
                "named \"%s\".\033[0m\n", editor->filename.data);
        changeState(editor, MAPEDIT_STATE_MENU);
        return;
    }

    const char header[9] = "r2Dtiles";
    int byte;

    for(uint8_t i = 0; i < 8 && ((byte = fgetc(file)) != EOF); ++i)
    {
        if(byte != header[i])
        {
            fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                    "\033[0m\n", editor->filename.data);
            return;
        }
    }

    size_t elements = 0;

    if((elements = fread(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", editor->filename.data);
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", editor->filename.data);
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", editor->filename.data);
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s."
                "\033[0m\n", editor->filename.data);
        return;
    }

    uint64_t tilecount = editor->layers * editor->mapWidth * editor->mapHeight;

    for(uint32_t i = 0; i < tilecount * 4 && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->tiles)[i] = (uint8_t)byte;
    }

    if(engine->planes[MAPEDIT_PLANE_TILESHEET].data)
    {
        river2D_destroyImage(&engine->planes[MAPEDIT_PLANE_TILESHEET]);
    }

    // NOTE: might not be BGRA
    engine->planes[MAPEDIT_PLANE_TILESHEET].data = imgsurf_load_ptr(file,
            IMGSURF_FILE_QOI, &engine->planes[MAPEDIT_PLANE_TILESHEET].width,
            &engine->planes[MAPEDIT_PLANE_TILESHEET].height, IMGSURF_CHANNELS_BGRA, 8);
    engine->planes[MAPEDIT_PLANE_TILESHEET].path = "imgsurf_load_ptr in loadProject";

    fclose(file);
}

f_internal void checkFilePickerButtons
(
    EngineData *engine,
    EditorData *editor
){
    if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    // TODO: allow ctrl+backspace to clear the whole thing

    // TODO: text scaling? wrapping? any thing of this sort?

    // TODO: display an indicator, so as to signify that you're in a file opener
    // dialog...

    // TODO: use editor->cursor

    if(engine->controls.keymap & MAPEDIT_BIT_BACKSPACE)
    {
        puddle_sv_trim((StringView*)&editor->filename, 1, SV_RIGHT);
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
                // sv_add_char(&editor->filename, 0x40 + i, SV_RIGHT);
                editor->filename.data[editor->cursor] = 0x40 + i;
                continue;
            }

            // const char *text = sv_add_char(&editor->filename, 0x61 + i, SV_RIGHT);
        }
    }

    if(engine->controls.keymap & MAPEDIT_BIT_ENTER)
    {
        editor->confirmed = true;
        engine->controls.keymap &= ~MAPEDIT_BIT_ENTER;
    }

    if(editor->confirmed)
    {
        loadProject(engine, editor);
        changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    engine->loadText(engine, &engine->planes[MAPEDIT_PLANE_CURRENTFILE],
                     (StringView*)&editor->filename, MAPEDIT_PLANE_FONT16,
                     16, 1, 0, 0);
}

void mapedit_update
(
    EngineData *engine,
    EditorData *editor
){
    if(editor->currentState == MAPEDIT_STATE_MENU)
    {
        drawMainMenu(engine, editor);
        checkMainMenuButtons(engine, editor);
    }
    else if(editor->currentState == MAPEDIT_STATE_EDIT)
    {
        drawEditor(engine, editor);
        checkEditorButtons(engine, editor);
    }
    else if(editor->currentState == MAPEDIT_STATE_LOAD)
    {
        drawFilePicker(engine);
        checkFilePickerButtons(engine, editor);
    }
    else
    {
        fprintf(stderr, "\033[31;1;7mERROR: invalid state: %u, previous: %u\033[0m\n",
                editor->currentState, editor->previousState);
    }
}

void mapedit_updateSelectSize
(
    EditorData *editor,
    bool       increase
){
    if(increase && editor->selectMult < 8)
    {
        ++editor->selectMult;
    }
    else if(!increase && editor->selectMult > 1)
    {
        --editor->selectMult;
    }
}

#define CONTROL_NOTFOUND 0
#define CONTROL_FOUND    1

f_internal uint8_t processButton_function
(
    River2D_ControlMap *controls,
    uint64_t           desired,
    uint64_t           button,
    uint64_t           bit,
    bool               isDown
){
    if(button == controls->buttoncodes[desired])
    {
        if(isDown)
        {
            controls->buttonmap |= bit;
        }
        else
        {
            controls->buttonmap &= ~bit;
        }

        return CONTROL_FOUND;
    }

    return CONTROL_NOTFOUND;
}

#define processButton(bmacro, bitmacro) \
processButton_function(controls, bmacro, button, bitmacro, isDown)

f_internal uint8_t processKey_function
(
    River2D_ControlMap *controls,
    uint64_t           desired,
    uint64_t           key,
    uint64_t           bit,
    bool               isDown
){
    if(key == controls->keycodes[desired])
    {
        if(isDown)
        {
            controls->keymap |= bit;
        }
        else
        {
            controls->keymap &= ~bit;
        }
        return CONTROL_FOUND;
    }

    return CONTROL_NOTFOUND;
}

#define processKey(kmacro, bitmacro) \
processKey_function(controls, kmacro, key, bitmacro, isDown)

void mapedit_processButtons
(
    EditorData         *editor,
    River2D_ControlMap *controls,
    uint64_t           button,
    bool               isDown
){
    if(processButton(MAPEDIT_BUTTON_LEFTM, MAPEDIT_BIT_LEFTM))
    {
        if(isDown)
        {
            editor->lastActionStart = river2D_queryTime();
        }

        return;
    }
    if(processButton(MAPEDIT_BUTTON_MIDDLEM, MAPEDIT_BIT_MIDDLEM)){ return; }
    if(processButton(MAPEDIT_BUTTON_RIGHTM,  MAPEDIT_BIT_RIGHTM )){ return; }

#ifdef DEBUG
    fprintf(stderr, "button pressed: %" PRIx64 "\n", button);
#endif
}

void mapedit_processKeys
(
    River2D_ControlMap *controls,
    uint64_t           key,
    bool               isDown
){
    if(processKey(MAPEDIT_KEY_ESCAPE,     MAPEDIT_BIT_ESCAPE    )){ return; }
    if(processKey(MAPEDIT_KEY_ENTER,      MAPEDIT_BIT_ENTER     )){ return; }
    if(processKey(MAPEDIT_KEY_TAB,        MAPEDIT_BIT_TAB       )){ return; }
    if(processKey(MAPEDIT_KEY_LSHIFT,     MAPEDIT_BIT_LSHIFT    )){ return; }
    if(processKey(MAPEDIT_KEY_RSHIFT,     MAPEDIT_BIT_RSHIFT    )){ return; }
    if(processKey(MAPEDIT_KEY_LCTRL,      MAPEDIT_BIT_LCTRL     )){ return; }
    if(processKey(MAPEDIT_KEY_RCTRL,      MAPEDIT_BIT_RCTRL     )){ return; }
    if(processKey(MAPEDIT_KEY_BACKSPACE,  MAPEDIT_BIT_BACKSPACE )){ return; }
    if(processKey(MAPEDIT_KEY_0,          MAPEDIT_BIT_0         )){ return; }
    if(processKey(MAPEDIT_KEY_1,          MAPEDIT_BIT_1         )){ return; }
    if(processKey(MAPEDIT_KEY_2,          MAPEDIT_BIT_2         )){ return; }
    if(processKey(MAPEDIT_KEY_3,          MAPEDIT_BIT_3         )){ return; }
    if(processKey(MAPEDIT_KEY_4,          MAPEDIT_BIT_4         )){ return; }
    if(processKey(MAPEDIT_KEY_5,          MAPEDIT_BIT_5         )){ return; }
    if(processKey(MAPEDIT_KEY_6,          MAPEDIT_BIT_6         )){ return; }
    if(processKey(MAPEDIT_KEY_7,          MAPEDIT_BIT_7         )){ return; }
    if(processKey(MAPEDIT_KEY_8,          MAPEDIT_BIT_8         )){ return; }
    if(processKey(MAPEDIT_KEY_9,          MAPEDIT_BIT_9         )){ return; }
    if(processKey(MAPEDIT_KEY_A,          MAPEDIT_BIT_A         )){ return; }
    if(processKey(MAPEDIT_KEY_B,          MAPEDIT_BIT_B         )){ return; }
    if(processKey(MAPEDIT_KEY_C,          MAPEDIT_BIT_C         )){ return; }
    if(processKey(MAPEDIT_KEY_D,          MAPEDIT_BIT_D         )){ return; }
    if(processKey(MAPEDIT_KEY_E,          MAPEDIT_BIT_E         )){ return; }
    if(processKey(MAPEDIT_KEY_F,          MAPEDIT_BIT_F         )){ return; }
    if(processKey(MAPEDIT_KEY_G,          MAPEDIT_BIT_G         )){ return; }
    if(processKey(MAPEDIT_KEY_H,          MAPEDIT_BIT_H         )){ return; }
    if(processKey(MAPEDIT_KEY_I,          MAPEDIT_BIT_I         )){ return; }
    if(processKey(MAPEDIT_KEY_J,          MAPEDIT_BIT_J         )){ return; }
    if(processKey(MAPEDIT_KEY_K,          MAPEDIT_BIT_K         )){ return; }
    if(processKey(MAPEDIT_KEY_L,          MAPEDIT_BIT_L         )){ return; }
    if(processKey(MAPEDIT_KEY_M,          MAPEDIT_BIT_M         )){ return; }
    if(processKey(MAPEDIT_KEY_N,          MAPEDIT_BIT_N         )){ return; }
    if(processKey(MAPEDIT_KEY_O,          MAPEDIT_BIT_O         )){ return; }
    if(processKey(MAPEDIT_KEY_P,          MAPEDIT_BIT_P         )){ return; }
    if(processKey(MAPEDIT_KEY_Q,          MAPEDIT_BIT_Q         )){ return; }
    if(processKey(MAPEDIT_KEY_R,          MAPEDIT_BIT_R         )){ return; }
    if(processKey(MAPEDIT_KEY_S,          MAPEDIT_BIT_S         )){ return; }
    if(processKey(MAPEDIT_KEY_T,          MAPEDIT_BIT_T         )){ return; }
    if(processKey(MAPEDIT_KEY_U,          MAPEDIT_BIT_U         )){ return; }
    if(processKey(MAPEDIT_KEY_V,          MAPEDIT_BIT_V         )){ return; }
    if(processKey(MAPEDIT_KEY_W,          MAPEDIT_BIT_W         )){ return; }
    if(processKey(MAPEDIT_KEY_X,          MAPEDIT_BIT_X         )){ return; }
    if(processKey(MAPEDIT_KEY_Y,          MAPEDIT_BIT_Y         )){ return; }
    if(processKey(MAPEDIT_KEY_Z,          MAPEDIT_BIT_Z         )){ return; }
    if(processKey(MAPEDIT_KEY_MINUS,      MAPEDIT_BIT_MINUS     )){ return; }
    if(processKey(MAPEDIT_KEY_EQUAL,      MAPEDIT_BIT_EQUAL     )){ return; }

#ifdef DEBUG
    fprintf(stderr, "key pressed: 0x%" PRIx64 "\n", key);
#endif
}

void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
){
    Dimensions dim = river2D_getWindowSize(engine);

    if(x > dim.width)
    {
        x = dim.width;
    }
    if(y > dim.height)
    {
        y = dim.height;
    }

    engine->controls.pointer.x = (float)x / (float)dim.width;
    engine->controls.pointer.y = (float)y / (float)dim.height;
}
