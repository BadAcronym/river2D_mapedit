#include "mapedit_main.h"

#include "river2D_main.h"

#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>

s_global EngineData *_engine;
s_global EditorData *_editor;

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
            _engine->running = false;
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
            _engine->context = BeginPaint(window, &paintStruct);

            _engine->bltBuffer(_engine);

            EndPaint(window, &paintStruct);
            ReleaseDC(_engine->window, _engine->context);
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

            AsciiKey key = {0};
            key.key = (uint8_t)wParam;
            key.raw = (uint8_t)wParam;

            if(key.key > 0x40 && key.key < 0x5B)
            {
                key.key += 0x20;
            }

            meProcessKeys(&_engine->controls, key, isKeyDown);
            break;
        }
        case WM_MOUSEMOVE:
        {
            meProcessPointer(_engine, (uint32_t)GET_X_LPARAM(lParam),
                                      (uint32_t)GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONDOWN:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE1, true);
            break;
        }
        case WM_LBUTTONUP:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE1, false);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE2, true);
            break;
        }
        case WM_RBUTTONUP:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE2, false);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE3, true);
            break;
        }
        case WM_MBUTTONUP:
        {
            meProcessButtons(_editor, &_engine->controls, RV_MOUSE3, false);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            int32_t scrollAmount = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            if(scrollAmount > 0)
            {
                meScroll(_editor, false);
                break;
            }
            else if(scrollAmount < 0)
            {
                meScroll(_editor, true);
                break;
            }
            break;
        }
        case WM_SETCURSOR:
        {
            if(LOWORD(lParam) == HTCLIENT)
            {
                _engine->currentCursor = 0;
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

    EditorData editor = {0};
    EngineData engine = {0};
    RiverImage planes[RV_MAX_PLANES] = {0};

    StringView libpath = cstr_sv(LIBPATH);
    rvResolveRenderer(&engine, libpath, RV_RENDERER_SOFTWARE);

    rvLoadConfig(&engine.config);
    engine.config.choices |= RV_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName     =  "River2D Map Editor";
    engine.instance       =  instance;

    rvInit(&engine, planes);
    meInit(&engine, &editor);

    _engine = &engine;
    _editor = &editor;

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

    // int64_t ns_threshold = (int64_t)(1e9f / (float)editor.desiredFPS);
    // for now, specify below
    int64_t ns_threshold = (int64_t)(1e9f / 240.0f);
    if(ns_threshold < 0)
    {
        ns_threshold = 0;
    }

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

        int64_t delta = (int64_t)(rvDeltaTime_now_ns(&editor.lastPresentTime));

        if(delta < ns_threshold)
        {
            // sleep...
            // int64_t durationNS = ns_threshold - delta;
            // struct timespec duration = {0};
            // duration.tv_sec  = durationNS / BILLION;
            // duration.tv_nsec = durationNS % BILLION;
            // nanosleep(&duration, NULL);
        }

        engine.context = GetDC(engine.window);

        meUpdate(&engine, &editor);
        mePresent(&engine, &editor);

        ReleaseDC(engine.window, engine.context);
    }

    return rvShutdown(&engine);
}
