#include "mapedit_main.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

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

    // BACKLOG: move this garbo somewhere lol

    uint32_t charsize     = 16;
    uint32_t total_width  = 13 * charsize;
    uint32_t total_height = 6  * (charsize + 10);

    engine->planes[MAPEDIT_PLANE_MENU].width  = total_width;
    engine->planes[MAPEDIT_PLANE_MENU].height = total_height;
    engine->planes[MAPEDIT_PLANE_MENU].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_MENU], " MAP EDITOR",  MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_MENU], " NEW PROJECT", MAPEDIT_PLANE_FONT16, 16, 1, 0, 50);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_MENU], "LOAD PROJECT", MAPEDIT_PLANE_FONT16, 16, 1, 0, 74);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_MENU], "    QUIT",     MAPEDIT_PLANE_FONT16, 16, 1, 0, 126);

    charsize     = 16;
    total_width  = 13 * charsize;
    total_height = charsize + 10;

    engine->planes[MAPEDIT_PLANE_SELECTTILE].width  = total_width;
    engine->planes[MAPEDIT_PLANE_SELECTTILE].height = total_height;
    engine->planes[MAPEDIT_PLANE_SELECTTILE].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], "SELECT TILE",  MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    charsize     = 16;
    total_width  = 6 * charsize;
    total_height = charsize + 10;

    engine->planes[MAPEDIT_PLANE_CLOSE].width  = total_width;
    engine->planes[MAPEDIT_PLANE_CLOSE].height = total_height;
    engine->planes[MAPEDIT_PLANE_CLOSE].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_CLOSE], "CLOSE",  MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    //transfer to mapedit_loadConfig at some point, when keybinds should be remappable
    // FIXME: the mouse dodn't work like this on windows... obviously
    engine->controls.keycodes[MAPEDIT_KEY_LEFTM]      = RIVER2D_MOUSE1;
    engine->controls.keycodes[MAPEDIT_KEY_MIDDLEM]    = RIVER2D_MOUSE2;
    engine->controls.keycodes[MAPEDIT_KEY_RIGHTM]     = RIVER2D_MOUSE3;
    engine->controls.keycodes[MAPEDIT_KEY_ESCAPE]     = river2D_interpretCharAsKey(0x1b);
    engine->controls.keycodes[MAPEDIT_KEY_QUIT]       = river2D_interpretCharAsKey('q');
    engine->controls.keycodes[MAPEDIT_KEY_TILEPICKER] = river2D_interpretCharAsKey('t');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER0]     = river2D_interpretCharAsKey('1');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER1]     = river2D_interpretCharAsKey('2');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER2]     = river2D_interpretCharAsKey('3');
    engine->controls.keycodes[MAPEDIT_KEY_LAYER3]     = river2D_interpretCharAsKey('4');

    // TODO: create function that takes a top left point, width/height and some text.
    // + pass a button pointer where all info is written to.
    // + pass an image, where the text that was given to the function is now.
    // + pass the fontsize and the function should scale the size of the area correctly, so that it
    // is backbuffer size agnostic. the only thing that should change is the (centered) passed coordinate
    // of the desired button.

    // FIXME: make canvas size agnostic

    editor->button_new.upperLeft.x   = 0.36f;
    editor->button_new.upperLeft.y   = 0.42;
    editor->button_new.lowerRight.x  = 0.65f;
    editor->button_new.lowerRight.y  = 0.46f;

    editor->button_load.upperLeft.x  = 0.33f;
    editor->button_load.upperLeft.y  = 0.49f;
    editor->button_load.lowerRight.x = 0.64f;
    editor->button_load.lowerRight.y = 0.53f;

    editor->button_quit.upperLeft.x  = 0.44f;
    editor->button_quit.upperLeft.y  = 0.63f;
    editor->button_quit.lowerRight.x = 0.54f;
    editor->button_quit.lowerRight.y = 0.67f;

    editor->button_tilepicker_close.upperLeft.x  = 0.1f;
    editor->button_tilepicker_close.upperLeft.y  = 0.84f;
    editor->button_tilepicker_close.lowerRight.x = 0.25f;
    editor->button_tilepicker_close.lowerRight.y = 0.89f;

    // WIP: load upfront for now
    river2D_loadImage("assets/tiles/tilesheet.qoi", &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_CHANNELS_BGRA, 8);

    editor->tilesize = 16;
    editor->layers   = 4;

    editor->map_width  = engine->config.canvas_width  / editor->tilesize;
    editor->map_height = engine->config.canvas_height / editor->tilesize;

    uint64_t tilecount = editor->layers * editor->map_width * editor->map_height;

    // TODO: make sure to resize this image when needed (when canvas dims or tilesize changes)
    editor->tiles = malloc(tilecount * sizeof(Tile));
    for(uint32_t i = 0; i < tilecount; ++i)
    {
        editor->tiles[i].x = UINT32_MAX;
        editor->tiles[i].y = UINT32_MAX;
    }
}

internal void drawMainMenu
(
    EngineData *engine,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_BACKGROUND], RIVER2D_PICTOP_OVER, 0, 0, 0, 0,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].width,
                           engine->planes[MAPEDIT_PLANE_BACKGROUND].height);

    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_MENU], RIVER2D_PICTOP_OVER,
                           engine->backbuffer.width  / 2 - engine->planes[MAPEDIT_PLANE_MENU].width  / 2,
                           engine->backbuffer.height / 2 - engine->planes[MAPEDIT_PLANE_MENU].height / 2,
                           0, 0,
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

        // TODO: (mapedit #2): handle new project button

        // FIXME: why does indexing into src (highlight image) make compositing fail here?
        // need better error msgs.

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_new.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_new.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, 190, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            editor->state = MAPEDIT_STATE_EDIT;
            engine->controls.keymap &= ~MAPEDIT_BIT_LEFTM;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_load))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        // TODO: (mapedit #2): handle load project button

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_load.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_load.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, 210, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            editor->state = MAPEDIT_STATE_LOAD;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_quit))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               (uint32_t)(editor->button_quit.upperLeft.x * engine->backbuffer.width),
                               (uint32_t)(editor->button_quit.upperLeft.y * engine->backbuffer.height + 20),
                               0, 0, 70, 5);

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
                if(editor->tiles[index].x != UINT32_MAX && editor->tiles[index].y != UINT32_MAX)
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
        editor->state = MAPEDIT_STATE_MENU;
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    // TODO: (mapedit #6): highlight in gridsize where cursor is currently
    // grid starts from top left of backbuffer

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

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTTILE], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 2 - engine->planes[MAPEDIT_PLANE_SELECTTILE].width  / 2,
                               engine->backbuffer.height / 9,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].width,
                               engine->planes[MAPEDIT_PLANE_SELECTTILE].height);

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_CLOSE], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width / 10 + 10,
                               engine->backbuffer.height - engine->backbuffer.height / 9 - 16,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_CLOSE].width,
                               engine->planes[MAPEDIT_PLANE_CLOSE].height);

        // TODO: load each and every file that is in assets/tiles, then display their thumbnails 🤔

        // WIP: just load the one file for now and use it here... lmfao
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

        // TODO: river2D_listFiles would be a great place to start, no? return a ; separated string as paths

        // TODO: scrollwheel changes size, 2x2 smallest, 96x96 biggest? only limitation is the highlighting, lol

        if(river2D_insideRect(&engine->controls.pointer, &editor->button_tilepicker_close))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.x * engine->backbuffer.width  + 6),
                                   (uint32_t)(editor->button_tilepicker_close.upperLeft.y * engine->backbuffer.height + 20),
                                   0, 0, 92, 5);

            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                editor->editorflags &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
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

    // BACKLOG: isolate view to a selected layer

    // TODO: display current picked tile as (or right next to) the cursor.

    // TODO: allow resizing the view to zoom in or out freely into the backbuffer.

    // TODO: allow resizing the backbuffer itself to be smaller or bigger.
    // always keep a copy of the largest backbuffer in memory, so data is not lost when
    // sizing down, then back up.

    // TODO: wheel or menu of recently used tiles and a hotbar with specific ones, somewhere.

    uint8_t tileX = (uint8_t)(engine->controls.pointer.x * engine->backbuffer.width  / editor->tilesize);
    uint8_t tileY = (uint8_t)(engine->controls.pointer.y * engine->backbuffer.height / editor->tilesize);

    // TODO: display outline around the current selected tile?

    river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET], RIVER2D_PICTOP_OVER,
                           tileX * editor->tilesize,
                           tileY * editor->tilesize,
                           editor->selectedX * editor->tilesize,
                           editor->selectedY * editor->tilesize,
                           editor->tilesize,   editor->tilesize);

    if(engine->controls.keymap & MAPEDIT_BIT_LAYER0)
    {
        editor->currentLayer = 0;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LAYER1)
    {
        editor->currentLayer = 1;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LAYER2)
    {
        editor->currentLayer = 2;
    }
    else if(engine->controls.keymap & MAPEDIT_BIT_LAYER3)
    {
        editor->currentLayer = 3;
    }

    if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
    {
        editor->tiles[editor->currentLayer * editor->map_width * editor->map_height + tileY * editor->map_width + tileX].x = editor->selectedX;
        editor->tiles[editor->currentLayer * editor->map_width * editor->map_height + tileY * editor->map_width + tileX].y = editor->selectedY;
    }

    //if(river2D_insideRect(&engine->controls.pointer, &editor->100button_someothereditorbutton))
    // {
    //     river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE]);
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
        editor->state = MAPEDIT_STATE_MENU;
        engine->controls.keymap &= ~MAPEDIT_BIT_ESCAPE;
        return;
    }

    // TODO: (mapedit #2): write simple file picker or path loader,
    // then after loading file, transition to editor state
}

// BACKLOG: let 'escape' from the main menu return to current state?
void mapedit_update
(
    EngineData *engine,
    EditorData *editor,
    void (*river2D_compositeImage)(EngineData *engine,  River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY,  uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth,   uint32_t cropHeight)
){
    if(editor->state == MAPEDIT_STATE_MENU)
    {
        drawMainMenu(engine, river2D_compositeImage);
        checkMainMenuButtons(engine, editor, river2D_compositeImage);
    }
    else if(editor->state == MAPEDIT_STATE_EDIT)
    {
        drawEditor(engine, editor, river2D_compositeImage);
        checkEditorButtons(engine, editor, river2D_compositeImage);
    }
    else if(editor->state == MAPEDIT_STATE_LOAD)
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
