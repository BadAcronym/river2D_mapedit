#include "mapedit_main.h"

#ifdef ASAN
        #define LIBPATH "./vendor/river2D/bin/asan/"
#else
    #ifdef DEBUG
        #define LIBPATH "./vendor/river2D/bin/debug/"
    #else
        #define LIBPATH "./vendor/river2D/bin/release/"
    #endif
#endif

int main
(
    void
){
    EditorData    editor = {0};
    EngineData    engine = {0};
    River2D_Image planes[RIVER2D_MAX_PLANES] = {0};

    StringView libpath = cstr_sv(LIBPATH);
    river2D_resolveRenderer(&engine, libpath, RIVER2D_RENDERER_SOFTWARE);

    river2D_loadConfig(&engine.config);
    engine.config.choices |= RIVER2D_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName = "River2D Map Editor";

    river2D_init(&engine, planes);
    mapedit_init(&engine, &editor);

    Atom WM_DELETE = XInternAtom(engine.display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(engine.display, engine.window, &WM_DELETE, 1);

    bool mapped = false;

    uint32_t resizeWidth  = engine.backbuffer.width;
    uint32_t resizeHeight = engine.backbuffer.height;

    while(engine.running)
    {
        while(XPending(engine.display) > 0)
        {
            XEvent event = {0};
            XNextEvent(engine.display, &event);
            switch(event.type)
            {
                case KeyPress:
                {
                    AsciiKey ascii = processXKey(&engine, &event);
                    mapedit_processKeys(&engine.controls, ascii, true);
                    break;
                }
                case KeyRelease:
                {
                    AsciiKey ascii = processXKey(&engine, &event);
                    mapedit_processKeys(&engine.controls, ascii, false);
                    break;
                }
                case ButtonPress:
                {
                    if(event.xbutton.button == RIVER2D_MOUSE4)
                    {
                        mapedit_scroll(&editor, false);
                        break;
                    }
                    else if(event.xbutton.button == RIVER2D_MOUSE5)
                    {
                        mapedit_scroll(&editor, true);
                        break;
                    }

                    mapedit_processButtons(&editor, &engine.controls,
                                           event.xbutton.button, true);
                    break;
                }
                case ButtonRelease:
                {
                    mapedit_processButtons(&editor, &engine.controls,
                                           event.xbutton.button, false);
                    break;
                }
                case MotionNotify:
                {
                    mapedit_processPointer(&engine,
                                           (uint32_t)event.xmotion.x,
                                           (uint32_t)event.xmotion.y);
                    break;
                }
                case ClientMessage:
                {
                    if(event.xclient.data.l[0] == (long)WM_DELETE)
                    {
                        engine.running = false;
                    }
                    break;
                }
                case ConfigureNotify:
                {
                    resizeWidth  = (uint32_t)event.xconfigure.width;
                    resizeHeight = (uint32_t)event.xconfigure.height;

                    if(resizeWidth  != engine.config.window_width  ||
                       resizeHeight != engine.config.window_height
                    ){
                        engine.config.window_width  = resizeWidth;
                        engine.config.window_height = resizeHeight;
                    }
                    break;
                }
                case UnmapNotify:
                {
                    mapped = false;
                    break;
                }
                case MapNotify:
                {
                    mapped  = true;
                    break;
                }
            }
        }

        if(mapped)
        {
            mapedit_update(&engine, &editor);

            // 243 -> 240 for the same rounding error reasons as islescape right now :p
            uint32_t desiredFPS   = 243;
            int64_t  ns_threshold = (int64_t)(1e9f / (double)(desiredFPS));

            int64_t delta = (int64_t)river2D_deltaTime_now_ns(&editor.lastPresentTime);

            if(delta < ns_threshold)
            {
                struct timespec duration = {0, ns_threshold - delta};
                nanosleep(&duration, NULL);
            }

            mapedit_present(&engine, &editor);
        }
    }

    river2D_shutdown(&engine);
    return mapedit_shutdown(&editor);
}
