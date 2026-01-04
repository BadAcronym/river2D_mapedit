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

    engine->planes[MAPEDIT_PLANE_SELECTSHEET].width  = total_width;
    engine->planes[MAPEDIT_PLANE_SELECTSHEET].height = total_height;
    engine->planes[MAPEDIT_PLANE_SELECTSHEET].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_SELECTSHEET], "SELECT SHEET",  MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    charsize     = 16;
    total_width  = 6 * charsize;
    total_height = charsize + 10;

    engine->planes[MAPEDIT_PLANE_CLOSE].width  = total_width;
    engine->planes[MAPEDIT_PLANE_CLOSE].height = total_height;
    engine->planes[MAPEDIT_PLANE_CLOSE].data   = calloc(total_width * total_height * RIVER2D_BPP, 1);
    river2D_loadText(engine, &engine->planes[MAPEDIT_PLANE_CLOSE], "CLOSE",  MAPEDIT_PLANE_FONT16, 16, 1, 0, 0);

    //transfer to mapedit_loadConfig at some point, when keybinds should be remappable
    engine->controls.keycodes[MAPEDIT_KEY_LEFTM]      = Button1;
    engine->controls.keycodes[MAPEDIT_KEY_MIDDLEM]    = Button2;
    engine->controls.keycodes[MAPEDIT_KEY_RIGHTM]     = Button3;
    engine->controls.keycodes[MAPEDIT_KEY_ESCAPE]     = 9;
    engine->controls.keycodes[MAPEDIT_KEY_QUIT]       = 24;
    engine->controls.keycodes[MAPEDIT_KEY_TILEPICKER] = 41;

    editor->button_new.upperLeft.x   = 0.428f;
    editor->button_new.upperLeft.y   = 0.455f;
    editor->button_new.lowerRight.x  = 0.57f;
    editor->button_new.lowerRight.y  = 0.485f;

    editor->button_load.upperLeft.x  = 0.415f;
    editor->button_load.upperLeft.y  = 0.49f;
    editor->button_load.lowerRight.x = 0.57f;
    editor->button_load.lowerRight.y = 0.515f;

    editor->button_quit.upperLeft.x  = 0.47f;
    editor->button_quit.upperLeft.y  = 0.56f;
    editor->button_quit.lowerRight.x = 0.52f;
    editor->button_quit.lowerRight.y = 0.59f;

    editor->button_tilepicker_close.upperLeft.x  = 0.1f;
    editor->button_tilepicker_close.upperLeft.y  = 0.86f;
    editor->button_tilepicker_close.lowerRight.x = 0.17f;
    editor->button_tilepicker_close.lowerRight.y = 0.89f;

    // WIP: load upfront for now
    river2D_loadImage("assets/tiles/scenery.qoi", &engine->planes[MAPEDIT_PLANE_TILESHEET_0], RIVER2D_CHANNELS_BGRA, 8);
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

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               editor->button_new.upperLeft.x * engine->backbuffer.width,
                               editor->button_new.upperLeft.y * engine->backbuffer.height + 25,
                               800, 400, 196, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            editor->state = MAPEDIT_STATE_EDIT;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_load))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        // TODO: (mapedit #2): handle load project button

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               editor->button_load.upperLeft.x * engine->backbuffer.width,
                               editor->button_load.upperLeft.y * engine->backbuffer.height + 25,
                               800, 400, 210, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            editor->state = MAPEDIT_STATE_LOAD;
        }
    }
    else if(river2D_insideRect(&engine->controls.pointer, &editor->button_quit))
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                               editor->button_quit.upperLeft.x * engine->backbuffer.width,
                               editor->button_quit.upperLeft.y * engine->backbuffer.height + 25,
                               800, 400, 72, 5);

        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            engine->running = false;
        }
    }
    else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);

        //TESTING: getting info on UI button locations
        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            fprintf(stderr, "mouse clicked @:\n");
            fprintf(stderr, "x: %f\n", engine->controls.pointer.x);
            fprintf(stderr, "y: %f\n", engine->controls.pointer.y);
        }
    }
}

internal void drawEditor
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
                               engine->backbuffer.width  / 1.25f,
                               engine->backbuffer.height / 1.25f);

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_SELECTSHEET], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 2 - engine->planes[MAPEDIT_PLANE_SELECTSHEET].width  / 2,
                               engine->backbuffer.height / 9,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_SELECTSHEET].width,
                               engine->planes[MAPEDIT_PLANE_SELECTSHEET].height);

        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_CLOSE], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width / 10 + 10,
                               engine->backbuffer.height - engine->backbuffer.height / 9 - 16,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_CLOSE].width,
                               engine->planes[MAPEDIT_PLANE_CLOSE].height);

        // TODO: load each and every file that is in assets/tiles, then display their thumbnails 🤔

        // WIP: just load the one file for now and use it here... lmfao
        river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_TILESHEET_0], RIVER2D_PICTOP_OVER,
                               engine->backbuffer.width  / 10 + 32,
                               engine->backbuffer.height / 10 + 32,
                               0, 0,
                               engine->planes[MAPEDIT_PLANE_TILESHEET_0].width,
                               engine->planes[MAPEDIT_PLANE_TILESHEET_0].height);

        Rect tilesheet = {0};
        tilesheet.upperLeft.x  = 0.095f + (double)(32.0f / (double)engine->backbuffer.width);
        tilesheet.upperLeft.y  = 0.095f + (double)(32.0f / (double)engine->backbuffer.height);
        tilesheet.lowerRight.x = tilesheet.upperLeft.x + (double)engine->planes[MAPEDIT_PLANE_TILESHEET_0].width  / (double)engine->backbuffer.width;
        tilesheet.lowerRight.y = tilesheet.upperLeft.y + (double)engine->planes[MAPEDIT_PLANE_TILESHEET_0].height / (double)engine->backbuffer.height;

        // if you click on a file, it will be loaded and your cursor will turn into a tile selector (32x32 default),
        // scrollwheel changes size, 2x2 smallest, sheet height or width (whichever smallest) biggest

        // TODO: river2D_listFiles would be a great place to start, no? return a ; separated string as paths

        // TODO: think about how to load file

        if(river2D_insideRect(&engine->controls.pointer, &editor->button_tilepicker_close))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            river2D_compositeImage(engine, &engine->planes[MAPEDIT_PLANE_HIGHLIGHT], RIVER2D_PICTOP_OVER,
                                   editor->button_tilepicker_close.upperLeft.x * engine->backbuffer.width  + 6,
                                   editor->button_tilepicker_close.upperLeft.y * engine->backbuffer.height + 25,
                                   800, 400, 92, 5);

            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                editor->editorflags &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
            }
        }
        else if(river2D_insideRect(&engine->controls.pointer, &tilesheet))
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);

            // TODO: show reticle with current sprite size that is going to be selected

            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                // TODO: compute which tile was selected and keep it safe somewhere!
                uint32_t tilesize = 32;

                editor->editorflags &= ~MAPEDIT_FLAG_BIT_TILEPICKER;
            }
        }
        else
        {
            river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT]);

            //TESTING: getting info on UI button locations
            if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
            {
                fprintf(stderr, "mouse clicked @:\n");
                fprintf(stderr, "x: %f\n", engine->controls.pointer.x);
                fprintf(stderr, "y: %f\n", engine->controls.pointer.y);
            }
        }

        // TODO: display current picked tile as (or right next to) the cursor.

        // TODO: wheel or menu of recently used tiles and a hotbar with specific ones, somewhere.
        return;
    }

    //if(river2D_insideRect(&engine->controls.pointer, &editor->100button_someothereditorbutton))
    // {
    //     river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER]);
    // }
    // else
    {
        river2D_changeCursor(engine, &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE]);

        //TESTING: getting info on UI button locations
        if(engine->controls.keymap & MAPEDIT_BIT_LEFTM)
        {
            fprintf(stderr, "mouse clicked @:\n");
            fprintf(stderr, "x: %f\n", engine->controls.pointer.x);
            fprintf(stderr, "y: %f\n", engine->controls.pointer.y);
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
        drawEditor(engine, river2D_compositeImage);
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
    else
    {
        fprintf(stderr, "key pressed: %lu\n", key);
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
