#include "mapedit_main.h"

// BACKLOG: future renderers
//river2D_vulkan.dll / libriver2D_vulkan.so
//river2D_d3d.dll    / libriver2D_d3d.so
//river2D_openGL.dll / libriver2D_opengl.so
//
//move the functions to their appropriate paths

// TODO: load/save animated tiles, not just static ones!
// TODO: for this, allow animating tiles in the first place. How to store that info?

int main
(
    void
){
    EditorData    editor = {0};
    EngineData    engine = {0};
    River2D_Image planes[RIVER2D_MAX_PLANES] = {0};

    // TODO: get rid of this function pointer loading nonsense, just pass the libpath and wanted renderer :p
    // think about where this would go, Engine or app side function pointer struct.

    river2D_resolveRenderer(&engine, RIVER2D_RENDERER_SOFTWARE);

    river2D_loadConfig(&engine.config);
    engine.config.choices |= RIVER2D_CHOICE_STATIC_CANVAS_BIT;
    engine.windowName = "river2D map editor";

    engine.river2D_init(&engine, planes);
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
                    mapedit_processKeys(&engine.controls, event.xkey.keycode, true);
                    break;
                }
                case KeyRelease:
                {
                    mapedit_processKeys(&engine.controls, event.xkey.keycode, false);
                    break;
                }
                case ButtonPress:
                {
                    mapedit_processKeys(&engine.controls, event.xbutton.button, true);
                    break;
                }
                case ButtonRelease:
                {
                    mapedit_processKeys(&engine.controls, event.xbutton.button, false);
                    break;
                }
                case MotionNotify:
                {
                    mapedit_processPointer(&engine, event.xmotion.x, event.xmotion.y);
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
                    resizeWidth    = event.xconfigure.width;
                    resizeHeight   = event.xconfigure.height;

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
            mapedit_update(&engine, &editor);
            engine.river2D_bltBuffer(&engine);
        }
    }

    engine.river2D_shutdown(&engine);
    return mapedit_shutdown(&editor);
}
