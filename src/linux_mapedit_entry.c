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

    StringView libpath = pdCstrSV(LIBPATH);
    rvResolveFunctions(&engine, libpath, RV_RENDERER_SOFTWARE);

    rvLoadConfig(&engine.config);
    engine.config.choices |= RV_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName = "River2D Map Editor";

    rvInit(&engine, planes);
    meInit(&engine, &editor);

    Atom WM_DELETE = engine.xInternAtom(engine.display, "WM_DELETE_WINDOW", false);
    engine.xSetWMProtocols(engine.display, engine.window, &WM_DELETE, 1);

    bool mapped = false;

    u32 resizeWidth  = engine.backbuffer.width;
    u32 resizeHeight = engine.backbuffer.height;

    while(engine.running)
    {
        while(engine.xPending(engine.display) > 0)
        {
            XEvent event = {0};
            engine.xNextEvent(engine.display, &event);
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
                    meProcessPointer(&engine, (u32)event.xmotion.x,
                                     (u32)event.xmotion.y);
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
                    resizeWidth  = (u32)event.xconfigure.width;
                    resizeHeight = (u32)event.xconfigure.height;

                    if(resizeWidth  != engine.config.windowWidth  ||
                       resizeHeight != engine.config.windowHeight
                    ){
                        engine.config.windowWidth  = resizeWidth;
                        engine.config.windowHeight = resizeHeight;
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
                    mapped = true;
                    break;
                }
            }
        }

        if(mapped)
        {
            meUpdate(&engine, &editor);

            // 243 -> 240 for the same rounding error reasons as islescape right now :p
            u32 desiredFPS   = 243;
            i64 ns_threshold = (i64)(1e9f / (f64)(desiredFPS));

            i64 delta = (i64)rvDeltaTime_now_ns(&editor.lastPresentTime);

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
