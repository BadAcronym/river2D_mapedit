#include "mapedit_main.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

// TODO: write logging function (or library, lmfao) that writes to file, as well as stderr
// copy some river3D code for that?

void mapedit_init
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX)
){
    river2D_loadImage("assets/background.qoi", &engine->planes[MAPEDIT_PLANE_BACKGROUND], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_BACKGROUND].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }

    river2D_loadImage("assets/black.qoi", &engine->planes[MAPEDIT_PLANE_VOID], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_VOID].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load black image!\033[0m\n");
    }

    river2D_loadImage("assets/highlight.qoi", &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_HIGHLIGHT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load highlight image!\033[0m\n");
    }

    river2D_loadImage("assets/cursor_default.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load default cursor!\033[0m\n");
    }
    river2D_loadImage("assets/cursor_hover.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_HOVER].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load hover cursor!\033[0m\n");
    }
    river2D_loadImage("assets/cursor_place.qoi", &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_PLACE].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load place cursor!\033[0m\n");
    }

    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].width  = 32;
    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].height = 32;
    engine->planes[MAPEDIT_PLANE_CURSOR_NULL].data   = calloc(32 * 32 * RIVER2D_BPP, 1);

    river2D_loadImage("assets/font_default_16.qoi", &engine->planes[MAPEDIT_PLANE_FONT16], RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_FONT16].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load font image!\033[0m\n");
    }

    engine->planes[MAPEDIT_PLANE_MENU].width  = engine->backbuffer.width;
    engine->planes[MAPEDIT_PLANE_MENU].height = engine->backbuffer.height;
    engine->planes[MAPEDIT_PLANE_MENU].data   = calloc(engine->backbuffer.width * engine->backbuffer.height * RIVER2D_BPP, 1);

    engine->planes[MAPEDIT_PLANE_SELECTTILE].width  = engine->backbuffer.width;
    engine->planes[MAPEDIT_PLANE_SELECTTILE].height = engine->backbuffer.height;
    engine->planes[MAPEDIT_PLANE_SELECTTILE].data   = calloc(engine->backbuffer.width * engine->backbuffer.height * RIVER2D_BPP, 1);

    //transfer to mapedit_loadConfig at some point, when keybinds should be remappable
    // TODO: add "save project" UI button in the menu, but only if a project is actually loaded

    // TODO: handle each letter as itself, for typing...
    engine->controls.keycodes[MAPEDIT_KEY_LEFTM]      = RIVER2D_MOUSE1;
    engine->controls.keycodes[MAPEDIT_KEY_MIDDLEM]    = RIVER2D_MOUSE2;
    engine->controls.keycodes[MAPEDIT_KEY_RIGHTM]     = RIVER2D_MOUSE3;
    engine->controls.keycodes[MAPEDIT_KEY_ESCAPE]     = river2D_interpretCharAsKey(0x1b);
    engine->controls.keycodes[MAPEDIT_KEY_QUIT]       = river2D_interpretCharAsKey('q');
    engine->controls.keycodes[MAPEDIT_KEY_SAVE]       = river2D_interpretCharAsKey('s');
    engine->controls.keycodes[MAPEDIT_KEY_TILEPICKER] = river2D_interpretCharAsKey('t');
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

    // JANKY: I'm loading text by creating a button, then overwriting it. maybe allow for float-centric text loading in the future?
    Coordinates point = { .x = 0.5f, .y = 0.2f };
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MENU], "RIVER2D MAP EDITOR",  MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new, river2D_loadText);
    point.y = 0.4f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MENU], "NEW PROJECT",  MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_new, river2D_loadText);
    point.y = 0.5f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MENU], "LOAD PROJECT", MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_load, river2D_loadText);
    point.y = 0.7f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_MENU], "QUIT", MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_quit, river2D_loadText);

    // TODO: parametrize the rest of the buttons & text
    point.x = 0.5f;
    point.y = 0.14f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], "SELECT TILE", MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_tilepicker_close, river2D_loadText);
    point.x = 0.175f;
    point.y = 0.86f;
    river2D_createButton(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], "CLOSE", MAPEDIT_PLANE_FONT16, 16, 1, point, &editor->button_tilepicker_close, river2D_loadText);

    // HACK: load upfront for now
    // TODO: load all .qoi files from folder, then append to one big tilesheet in memory
    river2D_loadImage("assets/tiles/tilesheet.qoi", &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_CHANNELS_BGRA, 8);

    // TODO: allow changing tilesize dynamically (but it's intentional, 1-tilesize-per-project type deal)
    editor->tilesize = 16;

    // allow 10 layers by default, any other layer you'd have to add to the UI selector
    editor->layers       = 10;
    editor->currentLayer = 1;

    editor->map_width  = engine->config.canvas_width  / editor->tilesize;
    editor->map_height = engine->config.canvas_height / editor->tilesize;

    uint64_t tilecount = editor->layers * editor->map_width * editor->map_height;

    // TODO: make sure to resize this image when needed (when canvas dims or tilesize changes)
    editor->tiles = malloc(tilecount * sizeof(Tile));
    for(uint32_t i = 0; i < tilecount; ++i)
    {
        editor->tiles[i].x = UINT16_MAX;
        editor->tiles[i].y = UINT16_MAX;
    }

    if(!editor->projectName)
    {
        editor->projectName = "unnamed_project";
    }

    editor->current_state = MAPEDIT_STATE_MENU;
}

int32_t mapedit_shutdown
(
    EditorData *editor
){
    free(editor->tiles);
    return 0;
}

internal void changeState
(
    EditorData *editor,
    uint8_t    nextState
){
    if(editor->current_state == nextState)
    {
        fprintf(stderr, "\n\033[33;1;7mWARNING: trying to change state to the same state.\033[0m\n");
        return;
    }

    editor->previous_state = editor->current_state;
    editor->current_state  = nextState;
}

// NOTE: need to validate string length of whatever user sets projectName to
// TODO: some user indication (notification) that the project has been saved
internal void saveCurrentProject
(
    EditorData *editor
){
    char *filename = malloc(256);
    sprintf(filename, "%s.rte", editor->projectName);

    FILE *file = fopen(filename, "wb");
    if(!file)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: could not open file for saving: %s.\033[0m\n", editor->projectName);
    }

    // BACKLOG: more sophisticated binary format, with RLE, at least, so sizes aren't as bloated

    uint64_t tilecount = editor->layers * editor->map_height * editor->map_width;
    fwrite(editor->tiles, sizeof(editor->tiles), tilecount, file);
    fclose(file);
}

internal void drawMainMenu
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t  pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].width,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].height);

    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_MENU], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_MENU].width,
                           engine->planes[MAPEDIT_PLANE_MENU].height);
}

internal void checkMainMenuButtons
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    if(engine->controls.keymap & MAPEDIT_BIT_QUIT)
    {
        engine->running = false;
        engine->controls.keymap &= ~MAPEDIT_BIT_QUIT;
        return;
    }

    if(river2D_insideRect(&engine->controls.pointer, &editor->button_new))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        double length = editor->button_new.lowerRight.x - editor->button_new.upperLeft.x;
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_new.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_new.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            if(editor->previous_state != MAPEDIT_STATE_NULL && editor->tiles)
            {
                uint64_t tilecount = editor->layers * editor->map_height * editor->map_width;
                for(uint32_t i = 0; i < tilecount; ++i)
                {
                    editor->tiles[i].x = UINT16_MAX;
                    editor->tiles[i].y = UINT16_MAX;
                }
            }

            changeState(editor, MAPEDIT_STATE_EDIT);
            engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_load))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        double length = editor->button_load.lowerRight.x - editor->button_load.upperLeft.x;
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_load.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_load.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            changeState(editor, MAPEDIT_STATE_LOAD);
            engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_quit))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        double length = editor->button_quit.lowerRight.x - editor->button_quit.upperLeft.x;
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_quit.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_quit.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, length * engine->backbuffer.width, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            engine->running = false;
        }
    }
    else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);

        // TESTING:
        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            fprintf(stderr, "clicked @ X: %f Y: %f\n", engine->controls.pointer.x, engine->controls.pointer.y);
            engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
}

internal void drawEditor
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_VOID], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);

    for(uint32_t z = 0; z < editor->layers; ++z)
    {
        for(uint32_t x = 0; x < editor->map_width; ++x)
        {
            for(uint32_t y = 0; y < editor->map_height; ++y)
            {
                uint64_t index = z * editor->map_width * editor->map_height + y * editor->map_width + x;
                if(editor->tiles[index].x != UINT16_MAX && editor->tiles[index].y != UINT16_MAX)
                {
                    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_PICTOP_OVER,
                                           x * editor->tilesize,
                                           y * editor->tilesize,
                                           editor->tiles[index].x * editor->tilesize,
                                           editor->tiles[index].y * editor->tilesize,
                                           editor->tilesize, editor->tilesize);
                }
            }
        }
    }
}

internal void checkEditorButtons
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    if(engine->controls.keymap & MAPEDIT_BIT_SAVE)
    {
        saveCurrentProject(editor);
        engine->controls.keymap &= ~MAPEDIT_BIT_SAVE;

        fprintf(stdout, "Project saved successfully.\n");
    }

    // TODO: (mapedit #6): make backbuffer moveable,
    // move grid along with it (so it might be offset)

    // TODO: (mapedit #2): add some descriptions or UI elements to delimit what mode one is in

    if(engine->controls.keymap & MAPEDIT_BIT_TILEPICKER)
    {
        editor->editorflags ^= MAPEDIT_FLAG_BIT_TILEPICKER;
        engine->controls.keymap &= ~MAPEDIT_BIT_TILEPICKER;
    }

    if(editor->editorflags & MAPEDIT_FLAG_BIT_TILEPICKER)
    {
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10,
                               engine->backbuffer.height / 10,
                               0, 0,
                               (uint32_t)(engine->backbuffer.width  / 1.25f),
                               (uint32_t)(engine->backbuffer.height / 1.25f));

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].width,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].height);

        // TODO: load each and every file that is in assets/tiles, then display their thumbnails 🤔

        // HACK: just load the one file for now and use it here... lmfao
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10 + 32,
                               engine->backbuffer.height / 10 + 32,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_TILESHEET].width,
                               engine->planes[MAPEDIT_PLANE_TILESHEET].height);

        Rect tilesheet = {0};
        tilesheet.upperLeft.x  = 0.095f + (double)(32.0f / (double)engine->backbuffer.width);
        tilesheet.upperLeft.y  = 0.095f + (double)(32.0f / (double)engine->backbuffer.height);
        tilesheet.lowerRight.x = tilesheet.upperLeft.x + (double)engine->planes[MAPEDIT_PLANE_TILESHEET].width  / (double)engine->backbuffer.width;
        tilesheet.lowerRight.y = tilesheet.upperLeft.y + (double)engine->planes[MAPEDIT_PLANE_TILESHEET].height / (double)engine->backbuffer.height;

        // TODO: load all files inside the specified folder, separate the spreadsheets visually but have them be one image

        // TODO: river2D_appendImage (either by x or y) would be super handy to have in general

        // TODO: scrollwheel changes size, 2x2 smallest, 96x96 biggest? only limitation is the highlighting, lol

        if(river2D_insideRect(&engine->controls.pointer, &editor->button_tilepicker_close))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            double length = editor->button_tilepicker_close.lowerRight.x - editor->button_tilepicker_close.upperLeft.x;
            river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.x * engine->backbuffer.width),
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.y * engine->backbuffer.height + 20),
                                   0, 0, length * engine->backbuffer.width, 5);

            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                editor->editorflags &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
                engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
            }
        }
        else if(river2D_insideRect(&engine->controls.pointer, &tilesheet))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_NULL]);

            double  deltaX = engine->controls.pointer.x - tilesheet.upperLeft.x;
            double  deltaY = engine->controls.pointer.y - tilesheet.upperLeft.y;
            uint8_t tileX  = (uint8_t)(deltaX * engine->backbuffer.width  / editor->tilesize);
            uint8_t tileY  = (uint8_t)(deltaY * engine->backbuffer.height / editor->tilesize);

            river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                                   (uint32_t)(engine->backbuffer.width  * (tilesheet.upperLeft.x + 0.0055f) + tileX * editor->tilesize),
                                   (uint32_t)(engine->backbuffer.height * (tilesheet.upperLeft.y + 0.006f)  + tileY * editor->tilesize),
                                   0, 0, editor->tilesize, editor->tilesize);

            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                editor->selectedX       =  tileX;
                editor->selectedY       =  tileY;
                editor->editorflags     &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
                engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
            }
        }
        else
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);
        }

        return;
    }

    // TODO: display the current layer (maybe lil UI to the left).

    // BACKLOG: allow isolating view to a selected layer

    // TODO: allow resizing the view to zoom in or out freely into the backbuffer.

    // TODO: allow resizing the backbuffer itself to be smaller or bigger.
    // always keep a copy of the largest backbuffer in memory, so data is not lost when
    // sizing down, then back up.

    // TODO: wheel or menu of recently used tiles and a hotbar with specific ones, somewhere.

    uint8_t tileX = (uint8_t)(engine->controls.pointer.x * engine->backbuffer.width  / editor->tilesize);
    uint8_t tileY = (uint8_t)(engine->controls.pointer.y * engine->backbuffer.height / editor->tilesize);

    // TODO: display outline around the current selected tile? pulsating, maybe

    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_PICTOP_OVER,
                           tileX * editor->tilesize,
                           tileY * editor->tilesize,
                           editor->selectedX * editor->tilesize,
                           editor->selectedY * editor->tilesize,
                           editor->tilesize,   editor->tilesize);

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

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            editor->tiles[editor->currentLayer * editor->map_width * editor->map_height + tileY * editor->map_width + tileX].x = editor->selectedX;
            editor->tiles[editor->currentLayer * editor->map_width * editor->map_height + tileY * editor->map_width + tileX].y = editor->selectedY;
        }
    }
}

internal void drawFilePicker
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_VOID], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_VOID].width,
                           engine->planes[MAPEDIT_PLANE_VOID].height);
}

internal void checkFilePickerButtons
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    if(engine->controls.keymap & MAPEDIT_BIT_ESCAPE)
    {
        changeState(editor, MAPEDIT_STATE_MENU);
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

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

    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        fprintf(stderr, "\033[31;1;7mERROR: could not open file: %s.\033[0m\n", filename);
        changeState(editor, MAPEDIT_STATE_EDIT);
        return;
    }

    uint64_t tilecount = editor->layers * editor->map_width * editor->map_height;
    int byte;
    for(uint32_t i = 0; i < tilecount * 4 && ((byte = fgetc(file)) != EOF); ++i)
    {
        ((uint8_t*)editor->tiles)[i] = byte;
    }

    fclose(file);
    free((void*)dirlist);

    if(filename)
    {
        free((void*)filename);
    }
    changeState(editor, MAPEDIT_STATE_EDIT);
}

// BACKLOG: let 'escape' from the main menu return to current state
void mapedit_update
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    if(editor->current_state == MAPEDIT_STATE_MENU)
    {
        drawMainMenu(engine, river2D_compositeImage);
        checkMainMenuButtons(engine, editor, river2D_compositeImage);
    }
    else if(editor->current_state == MAPEDIT_STATE_EDIT)
    {
        drawEditor(engine, editor, river2D_compositeImage);
        checkEditorButtons(engine, editor, river2D_compositeImage);
    }
    else if(editor->current_state == MAPEDIT_STATE_LOAD)
    {
        drawFilePicker(engine, river2D_compositeImage);
        checkFilePickerButtons(engine, editor, river2D_compositeImage);
    }
    else
    {
        fprintf(stderr, "\033[31;1;7mERROR: invalid state.\033[0m\n");
    }
}

void mapedit_processKeys
(
    bool               isDown,
    uint64_t           key,
    River2D_ControlMap *controls
){
    if(key == controls->keycodes[MAPEDIT_KEY_ESCAPE])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_ESCAPE;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_ESCAPE;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_SAVE])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_SAVE;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_SAVE;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_QUIT])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_QUIT;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_QUIT;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_TILEPICKER])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_TILEPICKER;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_TILEPICKER;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_LAYER0])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LAYER0;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LAYER0;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_LAYER1])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LAYER1;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LAYER1;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_LAYER2])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LAYER2;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LAYER2;
        }
    }
    else if(key == controls->keycodes[MAPEDIT_KEY_LAYER3])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LAYER3;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LAYER3;
        }
    }
    else
    {
        fprintf(stderr, "key pressed: %" PRIx64 "\n", key);
    }
}

void mapedit_processButtons
(
    bool               isDown,
    uint64_t           button,
    River2D_ControlMap *controls
){
    if(button == controls->keycodes[MAPEDIT_KEY_LEFTM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_LEFTM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(button == controls->keycodes[MAPEDIT_KEY_RIGHTM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_RIGHTM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_RIGHTM;
        }
    }
    else if(button == controls->keycodes[MAPEDIT_KEY_MIDDLEM])
    {
        if(isDown)
        {
            controls->keymap |= MAPEDIT_BIT_MIDDLEM;
        }
        else
        {
            controls->keymap &= ~MAPEDIT_BIT_MIDDLEM;
        }
    }
    else
    {
        fprintf(stderr, "button pressed: %" PRIx64 "\n", button);
    }
}

void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
){
    Dimensions dim = river2D_getWindowSize(engine);
    engine->controls.pointer.x = (double)x / dim.width;
    engine->controls.pointer.y = (double)y / dim.height;
}
