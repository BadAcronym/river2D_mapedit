#include "mapedit_main.h"
#include "imgsurf_main.h"
#include "string_view.h"

#include <math.h>
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

    StringView title_sv = cstr_sv("RIVER2D MAP EDITOR");
    StringView new_sv   = cstr_sv("NEW PROJECT");
    StringView load_sv  = cstr_sv("LOAD PROJECT");
    StringView save_sv  = cstr_sv("SAVE PROJECT");
    StringView quit_sv  = cstr_sv("QUIT");
    StringView close    = cstr_sv("CLOSE");

    // I'm loading text by creating a button, then overwriting it. pause/main
    // janky, but I don't mind it.
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

    editor->tilesize   = 32;
    editor->selectMult = 1;

    editor->layers       = 10;
    editor->currentLayer = 1;

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

    if(!editor->projectName.data)
    {
        editor->projectName = cstr_str("unnamed_project");
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

    editor->previousState = editor->currentState;
    editor->currentState  = nextState;
}

f_internal void saveCurrentProject
(
    EngineData *engine,
    EditorData *editor
){
    rvCompositeSettings comp = {0};
    comp.src                 = &engine->planes[MAPEDIT_PLANE_ICON_SAVING];
    comp.dst                 = &engine->backbuffer;
    comp.pictop              = RIVER2D_PICTOP_OVER;
    comp.cropWidth           = engine->planes[MAPEDIT_PLANE_ICON_SAVING].width;
    comp.cropHeight          = engine->planes[MAPEDIT_PLANE_ICON_SAVING].height;
    comp.offsetDstX          = 16;
    comp.offsetDstY          = 16;

    river2D_compositeImage(engine, &comp);

    engine->bltBuffer(engine);

    char *filename_cstr = malloc(256);
    sprintf(filename_cstr, PRI_SV".rte", ARG_SV(editor->projectName));

    FILE *file = fopen(filename_cstr, "wb");
    if(!file)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: could not open file for saving: "
                PRI_SV".\033[0m\n", ARG_SV(editor->projectName));
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

f_internal void checkMainMenuButtons
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

f_internal void checkEditorButtons
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
        saveCurrentProject(engine, editor);
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

f_internal void drawFilePicker
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

f_internal void checkFilePickerButtons
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
