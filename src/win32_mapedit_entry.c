#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>

#include "river2D_main.h"
#include "mapedit_main.h"

global EngineData *global_engine;
global EditorData *global_editor;

#ifdef ASAN
        #define LIBPATH "./vendor/river2D/bin/asan/"
#else
    #ifdef DEBUG
        #define LIBPATH "./vendor/river2D/bin/debug/"
    #else
        #define LIBPATH "./vendor/river2D/bin/release/"
    #endif
#endif

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

            global_engine->bltBuffer(global_engine);

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

            mapedit_processKeys(&global_engine->controls, wParam, isKeyDown);
            break;
        }
        case WM_MOUSEMOVE:
        {
            mapedit_processPointer(global_engine, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONDOWN:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE1, true);
            break;
        }
        case WM_LBUTTONUP:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE1, false);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE2, true);
            break;
        }
        case WM_RBUTTONUP:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE2, false);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE3, true);
            break;
        }
        case WM_MBUTTONUP:
        {
            mapedit_processButtons(global_editor, &global_engine->controls, RIVER2D_MOUSE3, false);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            int32_t scrollAmount = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            if(scrollAmount > 0)
            {
                mapedit_updateSelectSize(global_editor, true);
                break;
            }
            else if(scrollAmount < 0)
            {
                mapedit_updateSelectSize(global_editor, false);
                break;
            }
            break;
        }
        case WM_SETCURSOR:
        {
            if(LOWORD(lParam) == HTCLIENT)
            {
                global_engine->currentCursor = 0;
                return 1;
            }
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

    river2D_resolveRenderer(&engine, LIBPATH, RIVER2D_RENDERER_SOFTWARE);

    river2D_loadConfig(&engine.config);
    engine.config.choices |= RIVER2D_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName     =  "River2D Map Editor";
    engine.instance       =  instance;

    engine.init(&engine, planes);
    mapedit_init(&engine, &editor);

    global_engine = &engine;
    global_editor = &editor;

    // TODO: figure out the mapping window msgs

    // TODO: try to abstract this into river2D_openwindow without passing instance and shi, just what's already declared in the header

    WNDCLASSA wc     = {0};
    LPCSTR className = "MapeditClass";

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

        uint16_t desiredFPS = 144;

        River2D_Time now          = river2D_queryTime();
        int64_t      deltaMS      = river2D_deltaTime_ms(&editor.lastPresentTime, &now);
        double       ms_threshold = 1000 / (double)(desiredFPS);

        if(deltaMS < ms_threshold)
        {
            // struct timespec duration = {0, ns_threshold - deltaNS};
            // nanosleep(&duration, NULL);
            // TODO: nanosleep on windows, or 1ms precision
        }

        mapedit_update(&engine, &editor);
        engine.context = GetDC(engine.window);
        engine.bltBuffer(&engine);
        editor.lastPresentTime = now;
        ReleaseDC(engine.window, engine.context);
    }

    return engine.shutdown(&engine);
}
