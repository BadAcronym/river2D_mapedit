#include "mapedit_main.h"

#include "imgsurf_main.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

// BACKLOG: write logging function (or library, lmfao) that writes to file, as well as stderr
// copy some river3D code for that?

void mapedit_init
(
    EngineData *engine,
    EditorData *editor
){
    river2D_loadImage_file(engine, "assets/background.qoi", &engine->planes[MAPEDIT_PLANE_BACKGROUND], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_BACKGROUND].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/black.qoi", &engine->planes[MAPEDIT_PLANE_VOID], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_VOID].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load black image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/highlight.qoi", &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_HIGHLIGHT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load highlight image!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/saving.qoi", &engine->planes[MAPEDIT_PLANE_ICON_SAVING], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVING].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saving icon!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/saved.qoi", &engine->planes[MAPEDIT_PLANE_ICON_SAVED], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVED].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saved icon!\033[0m\n");
    }

    river2D_loadImage_file(engine, "assets/cursor_default.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load default cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/cursor_hover.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_HOVER].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load hover cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, "assets/cursor_place.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_PLACE].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load place cursor!\033[0m\n");
    }

    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].width  = 32;
    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].height = 32;
    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].data   = calloc(32 * 32 * RIVER2D_BPP, 1);

    river2D_loadImage_file(engine, "assets/font_default_16.qoi", &engine->planes[MAPEDIT_PLANE_FONT16], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_FONT16].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load font image!\033[0m\n");
    }

    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],   engine->backbuffer.width, engine->backbuffer.height);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU],  engine->backbuffer.width, engine->backbuffer.height);
    river2D_createImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], engine->backbuffer.width, engine->backbuffer.height);

    // BACKLOG: transfer to mapedit_loadConfig at some point, when keybinds should be remappable
    engine->controls.buttoncodes[MAPEDIT_BUTTON_LEFTM]      = RIVER2D_MOUSE1;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_MIDDLEM]    = RIVER2D_MOUSE2;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_RIGHTM]     = RIVER2D_MOUSE3;

    engine->controls.keycodes[MAPEDIT_KEY_ESCAPE]     = river2D_interpretCharAsKey(RIVER2D_ASCII_ESCAPE);
    engine->controls.keycodes[MAPEDIT_KEY_LSHIFT]     = river2D_interpretCharAsKey(RIVER2D_ASCII_LSHIFT);
    engine->controls.keycodes[MAPEDIT_KEY_LCTRL]      = river2D_interpretCharAsKey(RIVER2D_ASCII_LCTRL);
    engine->controls.keycodes[MAPEDIT_KEY_QUIT]       = river2D_interpretCharAsKey('q');
    engine->controls.keycodes[MAPEDIT_KEY_SAVE]       = river2D_interpretCharAsKey('s');
    engine->controls.keycodes[MAPEDIT_KEY_TILEPICKER] = river2D_interpretCharAsKey('t');
    engine->controls.keycodes[MAPEDIT_KEY_INC_SIZE]   = river2D_interpretCharAsKey('=');
    engine->controls.keycodes[MAPEDIT_KEY_RED_SIZE]   = river2D_interpretCharAsKey('-');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER0]     = river2D_interpretCharAsKey('0');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER1]     = river2D_interpretCharAsKey('1');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER2]     = river2D_interpretCharAsKey('2');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER3]     = river2D_interpretCharAsKey('3');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER4]     = river2D_interpretCharAsKey('4');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER5]     = river2D_interpretCharAsKey('5');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER6]     = river2D_interpretCharAsKey('6');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER7]     = river2D_interpretCharAsKey('7');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER8]     = river2D_interpretCharAsKey('8');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER9]     = river2D_interpretCharAsKey('9');
    engine->controls.keycodes[MAPEDIT_KEY_A]          = river2D_interpretCharAsKey('a');
    engine->controls.keycodes[MAPEDIT_KEY_B]          = river2D_interpretCharAsKey('b');
    engine->controls.keycodes[MAPEDIT_KEY_C]          = river2D_interpretCharAsKey('c');
    engine->controls.keycodes[MAPEDIT_KEY_D]          = river2D_interpretCharAsKey('d');
    engine->controls.keycodes[MAPEDIT_KEY_E]          = river2D_interpretCharAsKey('e');
    engine->controls.keycodes[MAPEDIT_KEY_F]          = river2D_interpretCharAsKey('f');
    engine->controls.keycodes[MAPEDIT_KEY_G]          = river2D_interpretCharAsKey('g');
    engine->controls.keycodes[MAPEDIT_KEY_H]          = river2D_interpretCharAsKey('h');
    engine->controls.keycodes[MAPEDIT_KEY_I]          = river2D_interpretCharAsKey('i');
    engine->controls.keycodes[MAPEDIT_KEY_J]          = river2D_interpretCharAsKey('j');
    engine->controls.keycodes[MAPEDIT_KEY_K]          = river2D_interpretCharAsKey('k');
    engine->controls.keycodes[MAPEDIT_KEY_L]          = river2D_interpretCharAsKey('l');
    engine->controls.keycodes[MAPEDIT_KEY_M]          = river2D_interpretCharAsKey('m');
    engine->controls.keycodes[MAPEDIT_KEY_N]          = river2D_interpretCharAsKey('n');
    engine->controls.keycodes[MAPEDIT_KEY_O]          = river2D_interpretCharAsKey('o');
    engine->controls.keycodes[MAPEDIT_KEY_P]          = river2D_interpretCharAsKey('p');
    engine->controls.keycodes[MAPEDIT_KEY_Q]          = river2D_interpretCharAsKey('q');
    engine->controls.keycodes[MAPEDIT_KEY_R]          = river2D_interpretCharAsKey('r');
    engine->controls.keycodes[MAPEDIT_KEY_S]          = river2D_interpretCharAsKey('s');
    engine->controls.keycodes[MAPEDIT_KEY_T]          = river2D_interpretCharAsKey('t');
    engine->controls.keycodes[MAPEDIT_KEY_U]          = river2D_interpretCharAsKey('u');
    engine->controls.keycodes[MAPEDIT_KEY_V]          = river2D_interpretCharAsKey('v');
    engine->controls.keycodes[MAPEDIT_KEY_W]          = river2D_interpretCharAsKey('w');
    engine->controls.keycodes[MAPEDIT_KEY_X]          = river2D_interpretCharAsKey('x');
    engine->controls.keycodes[MAPEDIT_KEY_Y]          = river2D_interpretCharAsKey('y');
    engine->controls.keycodes[MAPEDIT_KEY_Z]          = river2D_interpretCharAsKey('z');

    // JANKY: I'm loading text by creating a button, then overwriting it.
    // maybe allow for float-centric text loading in the future?
    Coordinates point = { .x = 0.5f, .y = 0.2f };

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU], "RIVER2D MAP EDITOR",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], "RIVER2D MAP EDITOR",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new);
    point.y = 0.4f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],  "NEW PROJECT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], "NEW PROJECT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new);

    point.y = 0.5f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],  "LOAD PROJECT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_load);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], "LOAD PROJECT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_load);
    point.y = 0.6f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], "SAVE PROJECT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_save);
    point.y = 0.8f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MAINMENU],  "QUIT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_quit);

    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_PAUSEMENU], "QUIT",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_quit);

    point.x = 0.175f;
    point.y = 0.86f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], "CLOSE",
                         MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_tilepicker_close);

    // HACK: load upfront for now
    // TODO: load all .qoi files from folder, then append to one big tilesheet in memory
    // BACKLOG: allow replacing this tilesheet by selecting one or multiple files with a file browser...
    river2D_loadImage_file(engine, "assets/tiles/tilesheet.qoi", &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_CHANNELS_BGRA, 8);

    // BACKLOG: allow decreasing/increasing this minimum tilesize
    editor->tilesize   = 8;
    editor->selectMult = 1;

    // allow 10 layers by default, any other layer you'd have to add to the UI selector
    // PERF: start with 1 layer, only poll and draw that layer, until there's any data written to the other layers 🤔
    // means to keep track of what layer has had data written to it, and only go through those??
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
        editor->actions[i].action_start.s  = 0;
        editor->actions[i].action_start.ns = 0;
    }

    // TODO: allow changing project name with a menu item or hotkey, pop-up textbox and user keyboard input
    if(!editor->projectName)
    {
        editor->projectName = "unnamed_project";
    }

    River2D_Time now                = river2D_queryTime();
    editor->lastPresentTime         = now;
    editor->lastSaveTime.s          = 1;
    editor->lastSaveTime.ns         = 1;
    engine->controls.lastScrollTime = now;

    editor->currentState = MAPEDIT_STATE_MENU;
}

int32_t mapedit_shutdown
(
    EditorData *editor
){
    free(editor->tiles);
    free(editor->actions);
    return 0;
}

internal void changeState
(
    EditorData *editor,
    uint8_t    nextState
){
    if(editor->currentState == nextState)
    {
        fprintf(stderr, "\n\033[33;1;7mWARNING: trying to change state to the same state: %u.\033[0m\n", editor->currentState);
        return;
    }

    editor->previousState = editor->currentState;
    editor->currentState  = nextState;
}

// TODO: add RLE? lose 1 bit of the 32 for every tile, just to make some uint32_t's a repeat of the last placed tile
// see if this is even worth it, it'd only be in exact runs
// find another way to do RLE maybe, one that scales well with lots of pairs of 2, 4, short runs that is

internal void saveCurrentProject
(
    EngineData *engine,
    EditorData *editor
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVING], &engine->backbuffer,
                           RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                           engine->planes[MAPEDIT_PLANE_ICON_SAVING].width,
                           engine->planes[MAPEDIT_PLANE_ICON_SAVING].height);

    engine->bltBuffer(engine);

    char *filename = malloc(256);
    sprintf(filename, "%s.rte", editor->projectName);

    FILE *file = fopen(filename, "wb");
    if(!file)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: could not open file for saving: %s.\033[0m\n", editor->projectName);
        free(filename);
        return;
    }

    const char header[9] = "r2Dtiles";
    fwrite(header, sizeof(header) - 1, 1, file);

    size_t elements = 0;

    if((elements = fwrite(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile. fwrite returned %zu, expected %u.\033[0m\n", elements, 1);
        return;
    }
    if((elements = fwrite(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to write header to savefile.\033[0m\n");
        return;
    }

    uint64_t tilecount = editor->layers * editor->mapHeight * editor->mapWidth;
    fwrite(editor->tiles, sizeof(Tile), tilecount, file);

    imgsurf_write_ptr(file, engine->planes[MAPEDIT_PLANE_TILESHEET].data, IMGSURF_FILE_QOI,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                      engine->planes[MAPEDIT_PLANE_TILESHEET].height,
                      IMGSURF_CHANNELS_BGRA, 8);

    fclose(file);

    free(filename);
    fprintf(stdout, "Project saved successfully.\n");

    editor->lastSaveTime = river2D_queryTime();
}

internal void drawMainMenu
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

    // BACKLOG: make these icons use some sort of opacity, fade-out animation (and / or animation in general)...
    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVED], &engine->backbuffer,
                               RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].width,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].height);
    }
}

internal void checkMainMenuButtons
(
    EngineData *engine,
    EditorData *editor
){
    if(engine->controls.keymap & MAPEDIT_BIT_QUIT)
    {
        engine->running = false;
        engine->controls.keymap &= ~MAPEDIT_BIT_QUIT;
        return;
    }

    if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE && editor->previousState != MAPEDIT_STATE_NULL)
    {
        changeState(editor, editor->previousState);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    if(river2D_insideRect(&engine->controls.pointer, &editor->button_new))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->button_new.lowerRight.x - editor->button_new.upperLeft.x;
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_new.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_new.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            if(editor->previousState != MAPEDIT_STATE_NULL && editor->tiles)
            {
                uint64_t tilecount = editor->layers * editor->mapHeight * editor->mapWidth;
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
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_load))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->button_load.lowerRight.x - editor->button_load.upperLeft.x;
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_load.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_load.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            changeState(editor, MAPEDIT_STATE_LOAD);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_quit))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->button_quit.lowerRight.x - editor->button_quit.upperLeft.x;
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_quit.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_quit.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            engine->running = false;
        }
    }
    else if(editor->previousState && river2D_insideRect(&engine->controls.pointer, &editor->button_save))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        float length = editor->button_save.lowerRight.x - editor->button_save.upperLeft.x;
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_save.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_save.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            saveCurrentProject(engine, editor);
            engine->controls.keymap    &= ~MAPEDIT_BIT_SAVE;
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);

        #ifdef DEBUG
        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            fprintf(stderr, "clicked @ X: %f Y: %f\n", engine->controls.pointer.x, engine->controls.pointer.y);
            engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
        }
        #endif
    }
}

internal void drawEditor
(
    EngineData *engine,
    EditorData *editor
){
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_VOID],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);

    for(uint32_t z = 0; z < editor->layers; ++z)
    {
        for(uint32_t x = 0; x < editor->mapWidth; ++x)
        {
            for(uint32_t y = 0; y < editor->mapHeight; ++y)
            {
                uint64_t index = z * editor->mapWidth * editor->mapHeight + y * editor->mapWidth + x;
                if(editor->tiles[index].x != UINT16_MAX && editor->tiles[index].y != UINT16_MAX)
                {
                    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET],
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

    engine->loadText(engine, &engine->planes[MAPEDIT_PLANE_CURRENTLAYER],
                     layerStr, MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_CURRENTLAYER], &engine->backbuffer,
                           RIVER2D_PICTOP_OVER, 608, 16, 0, 0,
                           engine->planes[MAPEDIT_PLANE_CURRENTLAYER].width,
                           engine->planes[MAPEDIT_PLANE_CURRENTLAYER].height);

    // BACKLOG: make these icons use some sort of opacity, fade-out animation (and / or animation in general)...
    float deltaMS = river2D_deltaTime_now_ms(&editor->lastSaveTime);
    if(deltaMS < 250)
    {
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_ICON_SAVED], &engine->backbuffer,
                               RIVER2D_PICTOP_OVER, 16, 16, 0, 0,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].width,
                               engine->planes[MAPEDIT_PLANE_ICON_SAVED].height);
    }
}

internal void incrementAction
(
    EditorData *editor
){
    if(++editor->currentAction > MAPEDIT_MAX_ACTIONS - 1)
    {
        editor->currentAction = 0;
    }
}

internal void decrementAction
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

internal void readAction
(
    EditorData *editor
){
    Action lastAction = editor->actions[editor->currentAction];
    editor->tiles[lastAction.map_index] = lastAction.prev_tile;
}

internal void writeAction
(
    EditorData   *editor,
    uint64_t     map_index,
    Tile         new_tile
){
    Tile prev_tile = editor->tiles[map_index];
    editor->tiles[map_index] = new_tile;

    editor->actions[editor->currentAction].action_start = editor->lastActionStart;
    editor->actions[editor->currentAction].map_index    = map_index;
    editor->actions[editor->currentAction].prev_tile    = prev_tile;
    editor->actions[editor->currentAction].new_tile     = new_tile;
}

// 150
// 150
// 150
// 150
// 150 <
// 200 <
// 0

// CURRENT: check the boundaries and make sure the pointer ends up in the same place. inc/decr is weird here

internal void undo
(
    EditorData *editor
){
    for(uint32_t undoCount = 0; undoCount < MAPEDIT_MAX_ACTIONS; ++undoCount)
    {
        decrementAction(editor);
        Action  currentAction = editor->actions[editor->currentAction];
        decrementAction(editor);

        Action  prevAction    = editor->actions[editor->currentAction];
        int64_t deltaNS       = river2D_deltaTime_ns(&prevAction.action_start, &currentAction.action_start);

        if(deltaNS < 0)
        {
            break;
        }

        readAction(editor);
    }
}

// TODO: do NOT re-do when the next action in sequence has a lower lastActionStart time than the current one.
internal void redo
(
    EditorData *editor
){
    fprintf(stderr, "TODO: REDO!\n");
}

internal void placeSelectedTiles
(
    EngineData *engine,
    EditorData *editor,
    uint16_t   tileX,
    uint16_t   tileY
){
    bool     break_outer = false;
    uint64_t sliceSize   = editor->mapWidth * editor->mapHeight;
    uint64_t topLeft     = editor->currentLayer * sliceSize + tileY * editor->mapWidth + tileX;
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

            // TODO: if new tile is old tile, skip writing completely

            writeAction(editor, index, new_tile);
            incrementAction(editor);
        }
    }
}

internal void checkEditorButtons
(
    EngineData *engine,
    EditorData *editor
){
    // TODO: (mapedit #6): make backbuffer moveable,
    // move grid along with it (so it might be offset)

    // TODO: (mapedit #2): add some descriptions or UI elements to delimit what mode one is in

    if(engine->controls.keymap & MAPEDIT_BIT_TILEPICKER)
    {
        editor->editorflags     ^= MAPEDIT_FLAG_BIT_TILEPICKER;
        engine->controls.keymap &= ~MAPEDIT_BIT_TILEPICKER;
    }

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
                               (uint32_t)(engine->backbuffer.width  / 1.25f),
                               (uint32_t)(engine->backbuffer.height / 1.25f));

        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].width,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].height);

        // TODO: load each and every file that is in assets/tiles, then display their thumbnails 🤔

        // HACK: just load the one file for now and use it here... lmfao
        engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                               &engine->backbuffer, RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10 + editor->tilesize,
                               engine->backbuffer.height / 10 + editor->tilesize,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                               engine->planes[MAPEDIT_PLANE_TILESHEET].height);

        Rect tilesheet = {0};
        tilesheet.upperLeft.x  = 0.095f + (float)((float)editor->tilesize / (float)engine->backbuffer.width);
        tilesheet.upperLeft.y  = 0.095f + (float)((float)editor->tilesize / (float)engine->backbuffer.height);
        tilesheet.lowerRight.x = tilesheet.upperLeft.x + (float)engine->planes[MAPEDIT_PLANE_TILESHEET].width  / (float)engine->backbuffer.width;
        tilesheet.lowerRight.y = tilesheet.upperLeft.y + (float)engine->planes[MAPEDIT_PLANE_TILESHEET].height / (float)engine->backbuffer.height;

        // TODO: load all files inside the specified folder, separate the spreadsheets visually but have them be one image

        // TODO: river2D_appendImage (either by x or y) would be super handy to have in general

        // TODO: scrollwheel changes size, 2x2 smallest, 96x96 biggest? only limitation is the highlighting, lol

        if(river2D_insideRect(&engine->controls.pointer, &editor->button_tilepicker_close))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            float length = editor->button_tilepicker_close.lowerRight.x - editor->button_tilepicker_close.upperLeft.x;
            engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                                   &engine->backbuffer, RIVER2D_PICTOP_OVER,
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.x * engine->backbuffer.width),
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.y * engine->backbuffer.height + 20),
                                   0, 0, length * engine->backbuffer.width, 5);

            if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
            {
                editor->editorflags        &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
                engine->controls.buttonmap &= ~MAPEDIT_BIT_LEFTM;
            }
        }
        else if(river2D_insideRect(&engine->controls.pointer, &tilesheet))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_NULL]);

            float   deltaX = engine->controls.pointer.x - tilesheet.upperLeft.x;
            float   deltaY = engine->controls.pointer.y - tilesheet.upperLeft.y;
            uint8_t  tileX  = (uint8_t)(deltaX * engine->backbuffer.width  / editor->tilesize);
            uint8_t  tileY  = (uint8_t)(deltaY * engine->backbuffer.height / editor->tilesize);

            uint16_t tilesheet_width  = engine->planes[MAPEDIT_PLANE_TILESHEET].width  / editor->tilesize;
            uint16_t tilesheet_height = engine->planes[MAPEDIT_PLANE_TILESHEET].height / editor->tilesize;

            if(tileX + editor->selectMult > tilesheet_width)
            {
                tileX = tilesheet_width - editor->selectMult;
            }
            if(tileY + editor->selectMult > tilesheet_height)
            {
                tileY = tilesheet_height - editor->selectMult;
            }

            if(engine->controls.keymap & MAPEDIT_BIT_INC_SIZE)
            {
                mapedit_updateSelectSize(editor, true);
                engine->controls.keymap &= ~MAPEDIT_BIT_INC_SIZE;
            }

            if(engine->controls.keymap & MAPEDIT_BIT_RED_SIZE)
            {
                mapedit_updateSelectSize(editor, false);
                engine->controls.keymap &= ~MAPEDIT_BIT_RED_SIZE;
            }

            engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                                   &engine->backbuffer, RIVER2D_PICTOP_OVER,
                                   (uint32_t)(engine->backbuffer.width  * (tilesheet.upperLeft.x + 0.0055f) + tileX * editor->tilesize),
                                   (uint32_t)(engine->backbuffer.height * (tilesheet.upperLeft.y + 0.006f)  + tileY * editor->tilesize),
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
    else if(engine->controls.keymap & MAPEDIT_BIT_SAVE)
    {
        saveCurrentProject(engine, editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_SAVE;
        return;
    }

    // BACKLOG: allow isolating view to a selected layer

    // TODO: allow resizing the view to zoom in or out freely into the backbuffer.
    // TODO: allow scrolling and panning the view, as well as expanding the tilemap

    // TODO: allow resizing the backbuffer itself to be smaller or bigger.
    // always keep a copy of the largest backbuffer in memory, so data is not lost when
    // sizing down, then back up.

    // TODO: wheel or menu of recently used tiles and a hotbar with specific ones, somewhere.

    uint16_t tileX = (uint16_t)(engine->controls.pointer.x * engine->backbuffer.width  / editor->tilesize);
    uint16_t tileY = (uint16_t)(engine->controls.pointer.y * engine->backbuffer.height / editor->tilesize);

    // TODO: display outline around the current selected tile

    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER,
                           tileX * editor->tilesize,
                           tileY * editor->tilesize,
                           editor->selectedX * editor->tilesize,
                           editor->selectedY * editor->tilesize,
                           editor->tilesize  * editor->selectMult,
                           editor->tilesize  * editor->selectMult);

    for(uint8_t i = 0; i < editor->layers; ++i)
    {
        if(engine->controls.keymap & (MAPEDIT_BIT_LAYER0 << i))
        {
            editor->currentLayer = i;
        }
    }

    //if(river2D_insideRect(&engine->controls.pointer, &editor->100button_someothereditorbutton))
    // {
    //     river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE]);

        if(engine->controls.buttonmap & MAPEDIT_BIT_LEFTM)
        {
            placeSelectedTiles(engine, editor, tileX, tileY);
        }
    }
}

internal void drawFilePicker
(
    EngineData *engine
){
    // void for now, file picker UI goes here
    engine->compositeImage(engine, &engine->planes[MAPEDIT_PLANE_VOID],
                           &engine->backbuffer, RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);
}

internal void loadProject
(
    EngineData *engine,
    EditorData *editor
){
    // TODO: later, this will be a filepicker screen with recent files, etc, (aseprite esc)
    // JANKY: load whatever file is "*.rte" in the current directory for now, instead of a filepicker
    const char *dirlist = river2D_listFiles(".");
    if(!dirlist)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to list directory!\033[0m\n");
        changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    char     *filename  = 0;
    uint32_t fileOffset = 0;
    for(uint32_t i = 0; dirlist[i] != '\0'; ++i)
    {
        if(dirlist[i] == ';')
        {
            fileOffset = i + 1;
            continue;
        }

        if(dirlist[i] == '\0' || dirlist[i + 1] == '\0' || dirlist[i + 2] == '\0' || dirlist[i + 3] == '\0')
        {
            break;
        }

        if(dirlist[i] == 'r' && dirlist[i + 1] == 't' && dirlist[i + 2] == 'e')
        {
            filename = (char*)malloc(255);

            uint8_t j = 0;
            for(; j < 255 && dirlist[fileOffset + j] != ';'; ++j)
            {
                filename[j] = dirlist[fileOffset + j];
            }
            filename[j] = '\0';

            break;
        }
    }

    if(!filename)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to find .rte file to load!\nlist: %s\n\n\033[0m\n", dirlist);
        changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    #ifdef DEBUG
    fprintf(stderr, "\nloading file: %s\n", filename);
    #endif

    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file: %s.\033[0m\n", filename);
        changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    const char header[9] = "r2Dtiles";
    int byte;

    for(uint8_t i = 0; i < 8 && ((byte = fgetc(file)) != EOF); ++i)
    {
        if(byte != header[i])
        {
            fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s.\033[0m\n", filename);
            return;
        }
    }

    size_t elements = 0;

    if((elements = fread(&editor->tilesize, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s.\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->mapWidth, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s.\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->mapHeight, 2, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s.\033[0m\n", filename);
        return;
    }
    if((elements = fread(&editor->layers, 1, 1, file)) != 1)
    {
        fprintf(stderr, "\033[31;1;7mERROR: failed to validate header in file: %s.\033[0m\n", filename);
        return;
    }

    uint64_t tilecount = editor->layers * editor->mapWidth * editor->mapHeight;

    for(uint32_t i = 0; i < tilecount * 4 && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->tiles)[i] = byte;
    }

    if(engine->planes[MAPEDIT_PLANE_TILESHEET].data)
    {
        river2D_destroyImage(&engine->planes[MAPEDIT_PLANE_TILESHEET]);
    }

    // NOTE: might not be BGRA
    engine->planes[MAPEDIT_PLANE_TILESHEET].data = imgsurf_load_ptr(file, IMGSURF_FILE_QOI, &engine->planes[MAPEDIT_PLANE_TILESHEET].width, &engine->planes[MAPEDIT_PLANE_TILESHEET].height, IMGSURF_CHANNELS_BGRA, 8);
    engine->planes[MAPEDIT_PLANE_TILESHEET].path = "imgsurf_load_ptr in loadProject";

    fclose(file);
    free((void*)dirlist);

    if(filename)
    {
        free((void*)filename);
    }
}

internal void checkFilePickerButtons
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

    loadProject(engine, editor);
    changeState(editor, MAPEDIT_STATE_EDIT);
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
        fprintf(stderr, "\033[31;1;7mERROR: invalid state: %u, previous: %u\033[0m\n", editor->currentState, editor->previousState);
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

internal uint8_t processButton
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

internal uint8_t processKey
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

void mapedit_processButtons
(
    EditorData         *editor,
    River2D_ControlMap *controls,
    uint64_t           button,
    bool               isDown
){
    if(processButton(controls, MAPEDIT_BUTTON_LEFTM, button, MAPEDIT_BIT_LEFTM, isDown))
    {
        if(isDown)
        {
            editor->lastActionStart = river2D_queryTime();
        }

        return;
    }
    if(processButton(controls, MAPEDIT_BUTTON_MIDDLEM,    button, MAPEDIT_BIT_MIDDLEM, isDown)){ return; }
    if(processButton(controls, MAPEDIT_BUTTON_RIGHTM,     button, MAPEDIT_BIT_RIGHTM,  isDown)){ return; }

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
    if(processKey(controls, MAPEDIT_KEY_ESCAPE,     key, MAPEDIT_BIT_ESCAPE,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LSHIFT,     key, MAPEDIT_BIT_LSHIFT,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LCTRL,      key, MAPEDIT_BIT_LCTRL,      isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_SAVE,       key, MAPEDIT_BIT_SAVE,       isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_QUIT,       key, MAPEDIT_BIT_QUIT,       isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_SAVE,       key, MAPEDIT_BIT_SAVE,       isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_TILEPICKER, key, MAPEDIT_BIT_TILEPICKER, isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_INC_SIZE,   key, MAPEDIT_BIT_INC_SIZE,   isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_RED_SIZE,   key, MAPEDIT_BIT_RED_SIZE,   isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER0,     key, MAPEDIT_BIT_LAYER0,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER1,     key, MAPEDIT_BIT_LAYER1,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER2,     key, MAPEDIT_BIT_LAYER2,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER3,     key, MAPEDIT_BIT_LAYER3,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER4,     key, MAPEDIT_BIT_LAYER4,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER5,     key, MAPEDIT_BIT_LAYER5,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER6,     key, MAPEDIT_BIT_LAYER6,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER7,     key, MAPEDIT_BIT_LAYER7,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER8,     key, MAPEDIT_BIT_LAYER8,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_LAYER9,     key, MAPEDIT_BIT_LAYER9,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_ZOOMIN,     key, MAPEDIT_BIT_ZOOMIN,     isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_ZOOMOUT,    key, MAPEDIT_BIT_ZOOMOUT,    isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_A,          key, MAPEDIT_BIT_A,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_B,          key, MAPEDIT_BIT_B,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_C,          key, MAPEDIT_BIT_C,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_D,          key, MAPEDIT_BIT_D,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_E,          key, MAPEDIT_BIT_E,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_F,          key, MAPEDIT_BIT_F,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_G,          key, MAPEDIT_BIT_G,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_H,          key, MAPEDIT_BIT_H,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_I,          key, MAPEDIT_BIT_I,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_J,          key, MAPEDIT_BIT_J,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_K,          key, MAPEDIT_BIT_K,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_L,          key, MAPEDIT_BIT_L,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_M,          key, MAPEDIT_BIT_M,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_N,          key, MAPEDIT_BIT_N,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_O,          key, MAPEDIT_BIT_O,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_P,          key, MAPEDIT_BIT_P,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_Q,          key, MAPEDIT_BIT_Q,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_R,          key, MAPEDIT_BIT_R,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_S,          key, MAPEDIT_BIT_S,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_T,          key, MAPEDIT_BIT_T,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_U,          key, MAPEDIT_BIT_U,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_V,          key, MAPEDIT_BIT_V,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_W,          key, MAPEDIT_BIT_W,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_X,          key, MAPEDIT_BIT_X,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_Y,          key, MAPEDIT_BIT_Y,          isDown)){ return; }
    if(processKey(controls, MAPEDIT_KEY_Z,          key, MAPEDIT_BIT_Z,          isDown)){ return; }

#ifdef DEBUG
    fprintf(stderr, "key pressed: %" PRIx64 "\n", key);
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

    engine->controls.pointer.x = (float)x / dim.width;
    engine->controls.pointer.y = (float)y / dim.height;
}
