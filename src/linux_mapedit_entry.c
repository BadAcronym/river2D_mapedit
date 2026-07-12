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
    EditorData editor = {0};
    EngineData engine = {0};
    RiverImage planes[RV_MAX_PLANES] = {0};

    StringView libpath = cstr_sv(LIBPATH);
    rvResolveRenderer(&engine, libpath, RV_RENDERER_SOFTWARE);

    rvLoadConfig(&engine.config);
    engine.config.choices |= RV_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName = "River2D Map Editor";

    rvInit(&engine, planes);
    meInit(&engine, &editor);

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
                    AsciiKey ascii = rvProcessXKey(&engine, &event);
                    meProcessKeys(&engine.controls, ascii, true);
                    break;
                }
                case KeyRelease:
                {
                    AsciiKey ascii = rvProcessXKey(&engine, &event);
                    meProcessKeys(&engine.controls, ascii, false);
                    break;
                }
                case ButtonPress:
                {
                    if(event.xbutton.button == RV_MOUSE4)
                    {
                        meScroll(&editor, false);
                        break;
                    }
                    else if(event.xbutton.button == RV_MOUSE5)
                    {
                        meScroll(&editor, true);
                        break;
                    }

                    meProcessButtons(&editor, &engine.controls,
                                           event.xbutton.button, true);
                    break;
                }
                case ButtonRelease:
                {
                    meProcessButtons(&editor, &engine.controls,
                                           event.xbutton.button, false);
                    break;
                }
                case MotionNotify:
                {
                    meProcessPointer(&engine,
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
            meUpdate(&engine, &editor);

            // 243 -> 240 for the same rounding error reasons as islescape right now :p
            uint32_t desiredFPS   = 243;
            int64_t  ns_threshold = (int64_t)(1e9f / (double)(desiredFPS));

            int64_t delta = (int64_t)rvDeltaTime_now_ns(&editor.lastPresentTime);

            if(delta < ns_threshold)
            {
                struct timespec duration = {0, ns_threshold - delta};
                nanosleep(&duration, NULL);
            }

            mePresent(&engine, &editor);
        }
    }

    rvShutdown(&engine);
    return meShutdown(&editor);
}
