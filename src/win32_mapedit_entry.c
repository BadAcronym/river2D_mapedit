#include <Windows.h>
#include <stdio.h>

#include "river2D_main.h"
#include "mapedit_main.h"

global EngineData *global_engine;

clang_ignore_unused

#define RIVER2D_INIT(name) void name(EngineData *engine, River2D_Image *planes)
typedef RIVER2D_INIT(river2D_init_);
RIVER2D_INIT(River2D_init_Stub)
{
    return;
}
global river2D_init_ *_river2D_init_ = River2D_init_Stub;
#define river2D_init _river2D_init_

#define RIVER2D_SHUT(name) int32_t name(EngineData *engine)
typedef RIVER2D_SHUT(river2D_shut_);
RIVER2D_SHUT(River2D_shut_Stub)
{
    return -1;
}
global river2D_shut_ *_river2D_shut_ = River2D_shut_Stub;
#define river2D_shutdown _river2D_shut_

#define RIVER2D_BLT(name) void name(EngineData *engine)
typedef RIVER2D_BLT(river2D_blt_);
RIVER2D_BLT(River2D_blt_Stub)
{
    return;
}
global river2D_blt_ *_river2D_blt_ = River2D_blt_Stub;
#define river2D_bltBuffer _river2D_blt_

#define RIVER2D_COMP(name) void name (EngineData *engine, River2D_Image *image, uint8_t pictop, uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX, uint32_t offsetSrcY, uint32_t cropWidth,  uint32_t cropHeight)
typedef RIVER2D_COMP(river2D_comp_);
RIVER2D_COMP(River2D_comp_Stub)
{
    return;
}
global river2D_comp_ *_river2D_comp_ = River2D_comp_Stub;
#define river2D_compositeImage _river2D_comp_

#define RIVER2D_LOADTEXT(name) void name(EngineData *engine, River2D_Image *image, const char *text, uint8_t font, uint16_t  charsize, uint32_t spacing, uint32_t offsetY, uint32_t offsetX)
typedef RIVER2D_LOADTEXT(river2D_text_);
RIVER2D_LOADTEXT(River2D_text_Stub)
{
    return;
}
global river2D_text_ *_river2D_text_ = River2D_text_Stub;
#define river2D_loadText _river2D_text_

clang_diagnostic_pop

#ifdef ASAN
    #define LIBPATH "./vendor/river2D/bin/asan/"
#else
    #ifdef DEBUG
        #define LIBPATH "./vendor/river2D/bin/debug/"
    #else
        #define LIBPATH "./vendor/river2D/bin/release/"
    #endif
#endif

internal void loadRenderer_software
(
    void
){
    SetDllDirectoryA(LIBPATH);
    HMODULE software = LoadLibraryA("river2Dsoftware.dll");
    if(!software)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load software renderer!\n");
        fprintf(stderr, "Tried to load from library path:" LIBPATH);
        fprintf(stderr, "\033[0m\n");
        return;
    }

    river2D_init = (river2D_init_*)GetProcAddress(software, "river2D_init");
    if(!river2D_init)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load symbol river2D_init!\033[0m\n");
        return;
    }
    river2D_shutdown = (river2D_shut_*)GetProcAddress(software, "river2D_shutdown");
    if(!river2D_shutdown)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load symbol river2D_shutdown!\033[0m\n");
        return;
    }
    river2D_bltBuffer = (river2D_blt_*)GetProcAddress(software, "river2D_bltBuffer");
    if(!river2D_bltBuffer)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load symbol river2D_bltBuffer!\033[0m\n");
        return;
    }
    river2D_compositeImage = (river2D_comp_*)GetProcAddress(software, "river2D_compositeImage");
    if(!river2D_compositeImage)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load symbol river2D_compositeImage!\033[0m\n");
        return;
    }
    river2D_loadText = (river2D_text_*)GetProcAddress(software, "river2D_loadText");
    if(!river2D_loadText)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to load symbol river2D_loadText!\033[0m\n");
        return;
    }
}

#if defined(DEBUG) || defined(ASAN)
int main
(
    void
){
    return WinMain(GetModuleHandleA(0), 0, GetCommandLineA(), 0);
}
#endif

LRESULT CALLBACK win32WindowCallback
(
    HWND   window,
    UINT   message,
    WPARAM wParam,
    LPARAM lParam
){
    switch(message)
    {
        case WM_DESTROY:
        {
            printf("WM_DESTROY\n");
            break;
        }
        case WM_CLOSE:
        {
            printf("WM_CLOSE\n");
            global_engine->running = false;
            break;
        }
        case WM_ACTIVATEAPP:
        {
            printf("WM_ACTIVATEAPP\n");
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            global_engine->context = BeginPaint(window, &paintStruct);

            river2D_bltBuffer(global_engine);

            EndPaint(window, &paintStruct);
            ReleaseDC(global_engine->window, global_engine->context);
            break;
        }
        case WM_KEYDOWN:
        {
        }
        case WM_KEYUP:
        {
            bool wasKeyDown = (lParam & (1 << 30)) != 0;
            bool isKeyDown  = (lParam & (1 << 31)) == 0;

            if(wasKeyDown == isKeyDown)
            {
                break;
            }

            // mapedit_processKeys(isKeyDown, uint64_t key, River2D_ControlMap *controls)
        }
        default:
        {
            return DefWindowProcA(window, message, wParam, lParam);
        }
    }

    return 0;
}

int CALLBACK WinMain
(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR     cmdline,
    int       cmdShow
){
    (void)cmdShow;
    (void)cmdline;
    (void)prevInstance;

    EditorData    editor = {0};
    EngineData    engine = {0};
    River2D_Image planes[RIVER2D_MAX_PLANES] = {0};

    loadRenderer_software();

    engine.instance   = instance;
    engine.windowName = "islescape";

    river2D_loadConfig(&engine.config);
    engine.config.choices |= RIVER2D_CHOICE_STATIC_CANVAS_BIT;
    river2D_init(&engine, planes);
    mapedit_init(&engine, &editor, river2D_loadText);

    global_engine = &engine;

    // TODO: figure out the mapping window msgs

    WNDCLASSA wc     = {0};
    LPCSTR className = "IslescapeClass";

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = win32WindowCallback;
    wc.hInstance     = engine.instance;
    wc.lpszClassName = className;

    if(!RegisterClassA(&wc))
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: Unable to register window class!\033[0m\n");
    };

    int x      = CW_USEDEFAULT;
    int y      = CW_USEDEFAULT;
    int width  = CW_USEDEFAULT;
    int height = CW_USEDEFAULT;

    engine.window = CreateWindowExA(0, wc.lpszClassName, engine.windowName,
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                    x, y, width, height,
                                    0, 0, instance, 0);
    while(engine.running)
    {
        MSG message;

        while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
        {
            if(message.message == WM_QUIT)
            {
                engine.running = false;
                break;
            }

            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        mapedit_update(&engine, &editor, river2D_compositeImage);

        engine.context = GetDC(engine.window);
        // if(mapped)
        // {
            river2D_bltBuffer(&engine);
        // }
        ReleaseDC(engine.window, engine.context);
    }

    return river2D_shutdown(&engine);
}
