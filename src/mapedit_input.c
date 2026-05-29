#include "mapedit_main.h"

#include <inttypes.h>

f_internal bool processButton_function
(
    River2D_ControlMap *controls,
    uint64_t           desired,
    uint64_t           button,
    uint64_t           bit,
    bool               isDown
){
    if(button == controls->buttoncodes[desired])
    {
        if(isDown)
        {
            controls->buttonmap |= bit;
        }
        else
        {
            controls->buttonmap &= ~bit;
        }

        return true;
    }

    return false;
}

#define processButton(bmacro, bitmacro) \
processButton_function(controls, bmacro, button, bitmacro, isDown)

f_internal void processKey_function
(
    River2D_ControlMap *controls,
    uint8_t            desired,
    AsciiKey           key,
    uint64_t           bit,
    bool               isDown
){
    if(isDown && key.key == controls->keycodes[desired])
    {
        controls->keymap |= bit;
        return;
    }

    if(key.key == controls->keycodes[desired])
    {
        controls->keymap &= ~bit;
    }
}

#define processKey(kmacro, bitmacro) \
processKey_function(controls, kmacro, key, bitmacro, isDown)

void mapedit_processButtons
(
    EditorData         *editor,
    River2D_ControlMap *controls,
    uint32_t           button,
    bool               isDown
){
    if(processButton(MAPEDIT_BUTTON_LEFTM, MAPEDIT_BIT_LEFTM))
    {
        if(isDown)
        {
            editor->lastActionStart = river2D_queryTime();
        }

        return;
    }
    if(processButton(MAPEDIT_BUTTON_MIDDLEM, MAPEDIT_BIT_MIDDLEM)){ return; }
    if(processButton(MAPEDIT_BUTTON_RIGHTM,  MAPEDIT_BIT_RIGHTM )){ return; }

#ifdef DEBUG
    fprintf(stderr, "button pressed: %" PRIx32 "\n", button);
#endif
}

void mapedit_processKeys
(
    River2D_ControlMap *controls,
    AsciiKey           key,
    bool               isDown
){
    if(isDown)
    {
        controls->ascii = (char)key.raw;
        if(controls->ascii < 0x20 || controls->ascii > 0x7E)
        {
            controls->ascii = 0x00;
        }
    }

    processKey(MAPEDIT_KEY_LSHIFT,     MAPEDIT_BIT_LSHIFT    );
    processKey(MAPEDIT_KEY_RSHIFT,     MAPEDIT_BIT_RSHIFT    );
    processKey(MAPEDIT_KEY_LCTRL,      MAPEDIT_BIT_LCTRL     );
    processKey(MAPEDIT_KEY_RCTRL,      MAPEDIT_BIT_RCTRL     );
    processKey(MAPEDIT_KEY_MENU,       MAPEDIT_BIT_MENU      );
    processKey(MAPEDIT_KEY_ENTER,      MAPEDIT_BIT_ENTER     );
    processKey(MAPEDIT_KEY_HOTBAR,     MAPEDIT_BIT_HOTBAR    );
    processKey(MAPEDIT_KEY_BACKSPACE,  MAPEDIT_BIT_BACKSPACE );
    processKey(MAPEDIT_KEY_DELETE,     MAPEDIT_BIT_DELETE    );
    processKey(MAPEDIT_KEY_LAYER0,     MAPEDIT_BIT_LAYER0    );
    processKey(MAPEDIT_KEY_LAYER1,     MAPEDIT_BIT_LAYER1    );
    processKey(MAPEDIT_KEY_LAYER2,     MAPEDIT_BIT_LAYER2    );
    processKey(MAPEDIT_KEY_LAYER3,     MAPEDIT_BIT_LAYER3    );
    processKey(MAPEDIT_KEY_LAYER4,     MAPEDIT_BIT_LAYER4    );
    processKey(MAPEDIT_KEY_LAYER5,     MAPEDIT_BIT_LAYER5    );
    processKey(MAPEDIT_KEY_LAYER6,     MAPEDIT_BIT_LAYER6    );
    processKey(MAPEDIT_KEY_LAYER7,     MAPEDIT_BIT_LAYER7    );
    processKey(MAPEDIT_KEY_LAYER8,     MAPEDIT_BIT_LAYER8    );
    processKey(MAPEDIT_KEY_LAYER9,     MAPEDIT_BIT_LAYER9    );
    processKey(MAPEDIT_KEY_REDO,       MAPEDIT_BIT_REDO      );
    processKey(MAPEDIT_KEY_UNDO,       MAPEDIT_BIT_UNDO      );
    processKey(MAPEDIT_KEY_DECREASE,   MAPEDIT_BIT_DECREASE  );
    processKey(MAPEDIT_KEY_INCREASE,   MAPEDIT_BIT_INCREASE  );
    processKey(MAPEDIT_KEY_SAVE,       MAPEDIT_BIT_SAVE      );
    processKey(MAPEDIT_KEY_QUIT,       MAPEDIT_BIT_QUIT      );
    processKey(MAPEDIT_KEY_TILEPICKER, MAPEDIT_BIT_TILEPICKER);
    processKey(MAPEDIT_KEY_WIREFRAME,  MAPEDIT_BIT_WIREFRAME);
    processKey(MAPEDIT_KEY_UP,         MAPEDIT_BIT_UP);
    processKey(MAPEDIT_KEY_DOWN,       MAPEDIT_BIT_DOWN);
    processKey(MAPEDIT_KEY_LEFT,       MAPEDIT_BIT_LEFT);
    processKey(MAPEDIT_KEY_RIGHT,      MAPEDIT_BIT_RIGHT);

#ifdef DEBUG
    fprintf(stderr, "key: %x\n", key.key);
    fprintf(stderr, "raw: %x\n", key.raw);
#endif
}

void mapedit_processPointer
(
    EngineData *engine,
    uint32_t   x,
    uint32_t   y
){
    Dimensions dim = river2D_getWindowSize(engine);

    if(x > dim.width)
    {
        x = dim.width;
    }
    if(y > dim.height)
    {
        y = dim.height;
    }

    engine->controls.pointer.x = (float)x / (float)dim.width;
    engine->controls.pointer.y = (float)y / (float)dim.height;
}

void mapedit_scroll
(
    EditorData *editor,
    bool       down
){
    if(editor->currentState != MAPEDIT_STATE_EDIT)
    {
        return;
    }

    if(down)
    {
        ++editor->viewScroll;
    }
    else if(editor->viewScroll)
    {
        --editor->viewScroll;
    }
}

void mapedit_updateSelectSize
(
    EditorData *editor,
    bool       increase
){
    if(increase && editor->selectMult < 8)
    {
        ++editor->selectMult;
    }
    else if(!increase && editor->selectMult > 1)
    {
        --editor->selectMult;
    }
}
