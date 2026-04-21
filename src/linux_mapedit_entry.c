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

    river2D_resolveRenderer(&engine, LIBPATH, RIVER2D_RENDERER_SOFTWARE);

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
                    uint8_t ascii = xkeyToAscii(&engine, &event);
                    mapedit_processKeys(&engine.controls, ascii, true);
                    break;
                }
                case KeyRelease:
                {
                    uint8_t ascii = xkeyToAscii(&engine, &event);
                    mapedit_processKeys(&engine.controls, ascii, false);
                    break;
                }
                case ButtonPress:
                {
                    if(event.xbutton.button == RIVER2D_MOUSE4)
                    {
                        mapedit_updateSelectSize(&editor, true);
                        break;
                    }
                    else if(event.xbutton.button == RIVER2D_MOUSE5)
                    {
                        mapedit_updateSelectSize(&editor, false);
                        break;
                    }

                    mapedit_processButtons(&editor, &engine.controls, event.xbutton.button, true);
                    break;
                }
                case ButtonRelease:
                {
                    mapedit_processButtons(&editor, &engine.controls, event.xbutton.button, false);
                    break;
                }
                case MotionNotify:
                {
                    mapedit_processPointer(&engine, (uint32_t)event.xmotion.x, (uint32_t)event.xmotion.y);
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

                    if(resizeWidth != engine.config.window_width || resizeHeight != engine.config.window_height)
                    {
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
            uint16_t desiredFPS = 144;

            River2D_Time now          = river2D_queryTime();
            int64_t      deltaNS      = river2D_deltaTime_ns(&editor.lastPresentTime, &now);
            double       ns_threshold = 1e9f / (double)(desiredFPS);

            if(deltaNS < (int64_t)ns_threshold)
            {
                struct timespec duration = {0, (int64_t)ns_threshold - deltaNS};
                nanosleep(&duration, NULL);
            }

            mapedit_update(&engine, &editor);
            engine.bltBuffer(&engine);
            editor.lastPresentTime = now;
        }
    }

    river2D_shutdown(&engine);
    return mapedit_shutdown(&editor);
}
