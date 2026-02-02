#include "mapedit_main.h"

// TODO: future renderers
//river2D_vulkan.dll / libriver2D_vulkan.so
//river2D_d3d.dll    / libriver2D_d3d.so
//river2D_openGL.dll / libriver2D_opengl.so
//
//move the functions to their appropriate paths
//
// TODO: read tile size, tiles and animations, save them to some format which can then be read by the game

#include <dlfcn.h>
#include <stdio.h>

#ifdef DEBUG
    #define LIBPATH "./vendor/river2D/bin/debug/"
#else
    #define LIBPATH "./vendor/river2D/bin/release/"
#endif

int main
(
    void
){
    // TODO: get rid of this function pointer loading nonsense, just pass the libpath and wanted renderer :p
    // think about where this would go, Engine or app side function pointer struct.

    char *error = 0;
    void *software = dlopen(LIBPATH "libriver2Dsoftware.so", RTLD_NOW);
    if(!software)
    {
        fprintf(stderr, "\033[31;1;7mERROR: Software renderer could not be loaded.\n");
        fputs(dlerror(), stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    void (*river2D_init)(EngineData *engine, River2D_Image *planes);
    *(void**)&river2D_init = dlsym(software, "river2D_init");
    if((error = dlerror()))
    {
        fprintf(stderr, "\033[31;1;7mERROR: Error while loading symbol river2D_init.\n");
        fputs(error, stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    int32_t (*river2D_shutdown)(EngineData *engine);
    *(void**)&river2D_shutdown = dlsym(software, "river2D_shutdown");
    if((error = dlerror()))
    {
        fprintf(stderr, "\033[31;1;7mERROR: Error while loading symbol river2D_shutdown.\n");
        fputs(error, stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    void (*river2D_bltBuffer)(EngineData *engine);
    *(void**)&river2D_bltBuffer = dlsym(software, "river2D_bltBuffer");
    if((error = dlerror()))
    {
        fprintf(stderr, "\033[31;1;7mERROR: Error while loading symbol river2D_bltBuffer.\n");
        fputs(error, stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    void (*river2D_compositeImage)(EngineData *engine, River2D_Image *image, uint8_t pictop,
                                   uint32_t offsetDstX, uint32_t offsetDstY, uint32_t offsetSrcX,
                                   uint32_t offsetSrcY, uint32_t cropWidth, uint32_t cropHeight);
    *(void**)&river2D_compositeImage = dlsym(software, "river2D_compositeImage");
    if((error = dlerror()))
    {
        fprintf(stderr, "\033[31;1;7mERROR: Error while loading symbol river2D_compositeImage.\n");
        fputs(error, stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    void (*river2D_loadText)(EngineData *engine, River2D_Image *image, const char *text,
                             uint8_t font, uint16_t  charsize, uint32_t spacing,
                             uint32_t offsetY, uint32_t offsetX);
    *(void**)&river2D_loadText = dlsym(software, "river2D_loadText");
    if((error = dlerror()))
    {
        fprintf(stderr, "\033[31;1;7mERROR: Error while loading symbol river2D_loadText.\n");
        fputs(error, stderr);
        fprintf(stderr, "\033[0m\n");
        return -1;
    }

    EditorData    editor = {0};
    EngineData    engine = {0};
    River2D_Image planes[RIVER2D_MAX_PLANES] = {0};

    river2D_loadConfig(&engine.config);
    engine.config.choices |= RIVER2D_CHOICE_STATIC_CANVAS_BIT;
    river2D_init(&engine, planes);
    mapedit_init(&engine, &editor, river2D_loadText);

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
                    mapedit_processKeys(true, event.xkey.keycode, &engine.controls);
                    break;
                }
                case KeyRelease:
                {
                    mapedit_processKeys(false, event.xkey.keycode, &engine.controls);
                    break;
                }
                case ButtonPress:
                {
                    mapedit_processButtons(true, event.xbutton.button, &engine.controls);
                    break;
                }
                case ButtonRelease:
                {
                    mapedit_processButtons(false, event.xbutton.button, &engine.controls);
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
            mapedit_update(&engine, &editor, river2D_compositeImage);
            river2D_bltBuffer(&engine);
        }
    }

    return river2D_shutdown(&engine);
}
