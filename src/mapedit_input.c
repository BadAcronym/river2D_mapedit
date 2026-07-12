#include "mapedit_main.h"

#include <inttypes.h>

f_internal bool processButton_function
(
    RiverControls *controls,
    uint64_t      desired,
    uint64_t      button,
    uint64_t      bit,
    bool          isDown
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
    RiverControls *controls,
    uint8_t       desired,
    AsciiKey      key,
    uint64_t      bit,
    bool          isDown
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
    EditorData    *editor,
    RiverControls *controls,
    uint32_t      button,
    bool          isDown
){
    if(processButton(ME_BUTTON_LEFTM, ME_BIT_LEFTM))
    {
        if(isDown)
        {
            editor->lastActionStart = rvQueryTime();
        }

        return;
    }
    if(processButton(ME_BUTTON_MIDDLEM, ME_BIT_MIDDLEM)){ return; }
    if(processButton(ME_BUTTON_RIGHTM,  ME_BIT_RIGHTM )){ return; }

#ifdef DEBUG
    fprintf(stderr, "button pressed: %" PRIx32 "\n", button);
#endif
}

void mapedit_processKeys
(
    RiverControls *controls,
    AsciiKey      key,
    bool          isDown
){
    if(isDown)
    {
        controls->ascii = (char)key.raw;
        if(controls->ascii < 0x20 || controls->ascii > 0x7E)
        {
            controls->ascii = 0x00;
        }
    }

    processKey(ME_KEY_LSHIFT,     ME_BIT_LSHIFT    );
    processKey(ME_KEY_RSHIFT,     ME_BIT_RSHIFT    );
    processKey(ME_KEY_LCTRL,      ME_BIT_LCTRL     );
    processKey(ME_KEY_RCTRL,      ME_BIT_RCTRL     );
    processKey(ME_KEY_MENU,       ME_BIT_MENU      );
    processKey(ME_KEY_ENTER,      ME_BIT_ENTER     );
    processKey(ME_KEY_HOTBAR,     ME_BIT_HOTBAR    );
    processKey(ME_KEY_BACKSPACE,  ME_BIT_BACKSPACE );
    processKey(ME_KEY_DELETE,     ME_BIT_DELETE    );
    processKey(ME_KEY_LAYER0,     ME_BIT_LAYER0    );
    processKey(ME_KEY_LAYER1,     ME_BIT_LAYER1    );
    processKey(ME_KEY_LAYER2,     ME_BIT_LAYER2    );
    processKey(ME_KEY_LAYER3,     ME_BIT_LAYER3    );
    processKey(ME_KEY_LAYER4,     ME_BIT_LAYER4    );
    processKey(ME_KEY_LAYER5,     ME_BIT_LAYER5    );
    processKey(ME_KEY_LAYER6,     ME_BIT_LAYER6    );
    processKey(ME_KEY_LAYER7,     ME_BIT_LAYER7    );
    processKey(ME_KEY_LAYER8,     ME_BIT_LAYER8    );
    processKey(ME_KEY_LAYER9,     ME_BIT_LAYER9    );
    processKey(ME_KEY_REDO,       ME_BIT_REDO      );
    processKey(ME_KEY_UNDO,       ME_BIT_UNDO      );
    processKey(ME_KEY_DECREASE,   ME_BIT_DECREASE  );
    processKey(ME_KEY_INCREASE,   ME_BIT_INCREASE  );
    processKey(ME_KEY_SAVE,       ME_BIT_SAVE      );
    processKey(ME_KEY_QUIT,       ME_BIT_QUIT      );
    processKey(ME_KEY_TILEPICKER, ME_BIT_TILEPICKER);
    processKey(ME_KEY_WIREFRAME,  ME_BIT_WIREFRAME );
    processKey(ME_KEY_UP,         ME_BIT_UP        );
    processKey(ME_KEY_DOWN,       ME_BIT_DOWN      );
    processKey(ME_KEY_LEFT,       ME_BIT_LEFT      );
    processKey(ME_KEY_RIGHT,      ME_BIT_RIGHT     );

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
    Dimensions dim = rvGetWindowSize(engine);

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
    if(editor->currentState != ME_STATE_EDIT)
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
