#include "mapedit_main.h"
#include "string_view.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

void mapedit_init
(
    EngineData *engine,
    EditorData *editor
){
    river2D_loadImage_file(engine, cstr_sv("assets/background.qoi"),
                           &engine->planes[MAPEDIT_PLANE_BACKGROUND],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_BACKGROUND].data)
    {
        fprintf(stderr,
                "\n\033[31;1;7mERROR: Unable to load background image!\033[0m\n");
    }

    river2D_loadImage_file(engine, cstr_sv("assets/black.qoi"),
                           &engine->planes[MAPEDIT_PLANE_VOID],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_VOID].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load black image!\033[0m\n");
    }

    river2D_loadImage_file(engine, cstr_sv("assets/highlight.qoi"),
                           &engine->planes[MAPEDIT_PLANE_HIGHLIGHT],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_HIGHLIGHT].data)
    {
        fprintf(stderr,
                "\n\033[31;1;7mERROR: Unable to load highlight image!\033[0m\n");
    }

    river2D_loadImage_file(engine, cstr_sv("assets/saving.qoi"),
                           &engine->planes[MAPEDIT_PLANE_ICON_SAVING],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVING].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saving icon!\033[0m\n");
    }
    river2D_loadImage_file(engine, cstr_sv("assets/saved.qoi"),
                           &engine->planes[MAPEDIT_PLANE_ICON_SAVED],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_ICON_SAVED].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load saved icon!\033[0m\n");
    }

    river2D_loadImage_file(engine, cstr_sv("assets/cursor_default.qoi"),
                           &engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_DEFAULT].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load default cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, cstr_sv("assets/cursor_hover.qoi"),
                           &engine->planes[MAPEDIT_PLANE_CURSOR_HOVER],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_HOVER].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load hover cursor!\033[0m\n");
    }
    river2D_loadImage_file(engine, cstr_sv("assets/cursor_place.qoi"),
                           &engine->planes[MAPEDIT_PLANE_CURSOR_PLACE],
                           RIVER2D_CHANNELS_BGRA, 8);
    if(!engine->planes[MAPEDIT_PLANE_CURSOR_PLACE].data)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load place cursor!\033[0m\n");
    }

    river2D_loadImage_file(engine, cstr_sv("assets/font_default_16.qoi"),
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

    engine->controls.buttoncodes[MAPEDIT_BUTTON_LEFTM]   = RIVER2D_MOUSE1;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_MIDDLEM] = RIVER2D_MOUSE2;
    engine->controls.buttoncodes[MAPEDIT_BUTTON_RIGHTM]  = RIVER2D_MOUSE3;

    engine->controls.keycodes[MAPEDIT_KEY_LSHIFT]       = RIVER2D_ASCII_LSHIFT;
    engine->controls.keycodes[MAPEDIT_KEY_RSHIFT]       = RIVER2D_ASCII_RSHIFT;
    engine->controls.keycodes[MAPEDIT_KEY_LCTRL]        = RIVER2D_ASCII_LCTRL;
    engine->controls.keycodes[MAPEDIT_KEY_RCTRL]        = RIVER2D_ASCII_RCTRL;
    engine->controls.keycodes[MAPEDIT_KEY_ENTER]        = RIVER2D_ASCII_ENTER;
    engine->controls.keycodes[MAPEDIT_KEY_BACKSPACE]    = RIVER2D_ASCII_BACKSPACE;
    engine->controls.keycodes[MAPEDIT_KEY_DELETE]       = RIVER2D_ASCII_DELETE;
    engine->controls.keycodes[MAPEDIT_KEY_MENU]         = RIVER2D_ASCII_ESCAPE;
    engine->controls.keycodes[MAPEDIT_KEY_HOTBAR]       = RIVER2D_ASCII_TAB;
    engine->controls.keycodes[MAPEDIT_KEY_UNDO]         = 'z';
    engine->controls.keycodes[MAPEDIT_KEY_REDO]         = 'y';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER0]       = '0';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER1]       = '1';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER2]       = '2';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER3]       = '3';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER4]       = '4';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER5]       = '5';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER6]       = '6';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER7]       = '7';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER8]       = '8';
    engine->controls.keycodes[MAPEDIT_KEY_LAYER9]       = '9';
    engine->controls.keycodes[MAPEDIT_KEY_REDO]         = 'y';
    engine->controls.keycodes[MAPEDIT_KEY_UNDO]         = 'z';
    engine->controls.keycodes[MAPEDIT_KEY_DECREASE]     = '-';
    engine->controls.keycodes[MAPEDIT_KEY_INCREASE]     = '=';
    engine->controls.keycodes[MAPEDIT_KEY_SAVE]         = 's';
    engine->controls.keycodes[MAPEDIT_KEY_QUIT]         = 'q';
    engine->controls.keycodes[MAPEDIT_KEY_TILEPICKER]   = 't';
    engine->controls.keycodes[MAPEDIT_KEY_UP]           = RIVER2D_ASCII_UP;
    engine->controls.keycodes[MAPEDIT_KEY_DOWN]         = RIVER2D_ASCII_DOWN;
    engine->controls.keycodes[MAPEDIT_KEY_LEFT]         = RIVER2D_ASCII_LEFT;
    engine->controls.keycodes[MAPEDIT_KEY_RIGHT]        = RIVER2D_ASCII_RIGHT;

    editor->tilesize     = 32;
    editor->selectMult   = 1;
    editor->layers       = 10;
    editor->currentLayer = 1;

    StringView title_sv  = cstr_sv("RIVER2D MAP EDITOR");
    StringView new_sv    = cstr_sv("NEW PROJECT");
    StringView load_sv   = cstr_sv("LOAD PROJECT");
    StringView save_sv   = cstr_sv("SAVE PROJECT");
    StringView saveas_sv = cstr_sv("SAVE AS");
    StringView quit_sv   = cstr_sv("QUIT");
    StringView close_sv  = cstr_sv("CLOSE");

    // JANKY: I'm loading text by creating a button, then overwriting it, like for the
    // title.

    ButtonSettings set = {0};
    set.img      = &engine->planes[MAPEDIT_PLANE_MAINMENU];
    set.name     = &title_sv;
    set.font     = MAPEDIT_PLANE_FONT16;
    set.charsize = 16;
    set.spacing  = 1;
    set.point.x  = 0.5f;
    set.point.y  = 0.2f;
    set.button   = &editor->new_b;
    river2D_createButton(engine, &set);

    set.img = &engine->planes[MAPEDIT_PLANE_PAUSEMENU];
    river2D_createButton(engine, &set);

    set.img     = &engine->planes[MAPEDIT_PLANE_MAINMENU];
    set.name    = &new_sv;
    set.point.y = 0.4f;
    river2D_createButton(engine, &set);

    set.img = &engine->planes[MAPEDIT_PLANE_PAUSEMENU];
    river2D_createButton(engine, &set);

    set.img     = &engine->planes[MAPEDIT_PLANE_MAINMENU];
    set.name    = &load_sv;
    set.point.y = 0.48f;
    set.button  = &editor->load_b;
    river2D_createButton(engine, &set);

    set.img = &engine->planes[MAPEDIT_PLANE_PAUSEMENU];
    river2D_createButton(engine, &set);

    set.name    = &save_sv;
    set.point.y = 0.56f;
    set.button  = &editor->save_b;
    river2D_createButton(engine, &set);

    set.name    = &saveas_sv;
    set.point.y = 0.64f;
    set.button  = &editor->saveas_b;
    river2D_createButton(engine, &set);

    set.img     = &engine->planes[MAPEDIT_PLANE_MAINMENU];
    set.name    = &quit_sv;
    set.point.y = 0.8f;
    set.button  = &editor->quit_b;
    river2D_createButton(engine, &set);

    set.img = &engine->planes[MAPEDIT_PLANE_PAUSEMENU];
    river2D_createButton(engine, &set);

    float bufHeight = (float)engine->backbuffer.height;

    set.name      = &close_sv;
    set.img       = &engine->planes[MAPEDIT_PLANE_SELECTTILE];
    set.point.x   = 0.1f + set.charsize / (bufHeight * 2);
    set.point.y   = 0.9f - set.charsize / bufHeight;
    set.button    = &editor->close_b;
    set.alignment = RIVER2D_ALIGN_BOTTOMLEFT;
    river2D_createButton(engine, &set);

    StringView dir = cstr_sv("assets/custom/");
    StringView ls  = river2D_listFiles(dir);

    StringView folder = cstr_sv("assets/custom/");

    for(uint16_t i = 0; i < dir.size; ++i)
    {
        StringView file = sv_find_by_delim(ls, ';', i);
        if(!file.data || !file.size)
        {
            break;
        }

        StringView expanded = cstr_sv(sv_concat(folder, file));

        if(!sv_find(cstr_sv(".qoi"), expanded))
        {
            free((void*)expanded.data);
            continue;
        }

        River2D_Image tmp;

        river2D_loadImage_file(engine, expanded, &tmp, RIVER2D_CHANNELS_BGRA, 8);
        river2D_appendImage(engine, &tmp, &engine->planes[MAPEDIT_PLANE_TILESHEET],
                            RIVER2D_VERTICAL);

        river2D_destroyImage(&tmp);
        free((void*)expanded.data);
    }

    free((void*)ls.data);

    editor->mapWidth  = engine->config.canvas_width  / editor->tilesize;
    editor->mapHeight = engine->config.canvas_height / editor->tilesize;

    uint64_t tilecount = editor->layers * editor->mapWidth * editor->mapHeight;
    editor->tiles = malloc(tilecount * sizeof(Tile));
    for(uint32_t i = 0; i < tilecount; ++i)
    {
        editor->tiles[i].flags |= MAPEDIT_BIT_INVALID;
    }

    editor->actions = malloc(MAPEDIT_MAX_ACTIONS * sizeof(Action));
    for(uint32_t i = 0; i < MAPEDIT_MAX_ACTIONS; ++i)
    {
        editor->actions[i].stroke_start.s  = INT64_MIN;
        editor->actions[i].stroke_start.ns = INT64_MIN;
    }

    if(!editor->inputBuffer.data)
    {
        editor->inputBuffer = cstr_str("unnamed_project");
    }

    River2D_Time now                = river2D_queryTime();
    editor->lastPresentTime         = now;
    editor->lastSaveTime.s          = 1;
    editor->lastSaveTime.ns         = 1;
    engine->controls.lastScrollTime = now;

    editor->currentState = MAPEDIT_STATE_MENU;
    char *buf = calloc(256, 1);
    editor->inputBuffer.data = buf;
    editor->inputBuffer.size = 256;
}

int32_t mapedit_shutdown
(
    EditorData *editor
){
    free(editor->tiles);
    free(editor->actions);
    free((void*)editor->inputBuffer.data);
    return 0;
}

void mapedit_changeState
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

    if(nextState == MAPEDIT_STATE_MENU)
    {
        editor->i_button = 0;
    }

    editor->previousState = editor->currentState;
    editor->currentState  = nextState;
}

void mapedit_update
(
    EngineData *engine,
    EditorData *editor
){
    if(editor->currentState == MAPEDIT_STATE_MENU)
    {
        mapedit_drawMainMenu(engine, editor);
        mapedit_pollMainMenu(engine, editor);
    }
    else if(editor->currentState == MAPEDIT_STATE_EDIT)
    {
        mapedit_drawEditor(engine, editor);
        mapedit_pollEditor(engine, editor);
    }
    else if(editor->currentState == MAPEDIT_STATE_LOAD)
    {
        mapedit_drawFilePicker(engine);
        mapedit_pollLoadFile(engine, editor);
    }
    else if(editor->currentState == MAPEDIT_STATE_SAVEAS)
    {
        mapedit_drawFilePicker(engine);
        mapedit_pollSaveFile(engine, editor);
    }
    else
    {
        fprintf(stderr, "\033[31;1;7mERROR: invalid state: %u, previous: %u\033[0m\n",
                editor->currentState, editor->previousState);
    }
}
