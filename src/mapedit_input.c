#include "mapedit_main.h"

#include <inttypes.h>

#define CONTROL_NOTFOUND 0
#define CONTROL_FOUND    1

f_internal uint8_t processButton_function
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

        return CONTROL_FOUND;
    }

    return CONTROL_NOTFOUND;
}

#define processButton(bmacro, bitmacro) \
processButton_function(controls, bmacro, button, bitmacro, isDown)

f_internal uint8_t processKey_function
(
    River2D_ControlMap *controls,
    uint8_t            desired,
    uint8_t            key,
    uint64_t           bit,
    bool               isDown
){
    fprintf(stderr, "desired: %u\n", desired);
    fprintf(stderr, "key: %u\n", key);
    fprintf(stderr, "bit: 0x%lx\n", bit);

    if(key == controls->keycodes[desired])
    {
        if(isDown)
        {
            controls->keymap |= bit;
        }
        else
        {
            controls->keymap &= ~bit;
        }
        return CONTROL_FOUND;
    }

    return CONTROL_NOTFOUND;
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
    fprintf(stderr, "button pressed: %" PRIx64 "\n", button);
#endif
}

void mapedit_processKeys
(
    River2D_ControlMap *controls,
    uint8_t            key,
    bool               isDown
){
    if(!key)
    {
        return;
    }

    if(processKey(MAPEDIT_KEY_ESCAPE,       MAPEDIT_BIT_ESCAPE       )){ return; }
    if(processKey(MAPEDIT_KEY_ENTER,        MAPEDIT_BIT_ENTER        )){ return; }
    if(processKey(MAPEDIT_KEY_TAB,          MAPEDIT_BIT_TAB          )){ return; }
    if(processKey(MAPEDIT_KEY_LSHIFT,       MAPEDIT_BIT_LSHIFT       )){ return; }
    if(processKey(MAPEDIT_KEY_RSHIFT,       MAPEDIT_BIT_RSHIFT       )){ return; }
    if(processKey(MAPEDIT_KEY_LCTRL,        MAPEDIT_BIT_LCTRL        )){ return; }
    if(processKey(MAPEDIT_KEY_RCTRL,        MAPEDIT_BIT_RCTRL        )){ return; }
    if(processKey(MAPEDIT_KEY_BACKSPACE,    MAPEDIT_BIT_BACKSPACE    )){ return; }
    if(processKey(MAPEDIT_KEY_0,            MAPEDIT_BIT_0            )){ return; }
    if(processKey(MAPEDIT_KEY_1,            MAPEDIT_BIT_1            )){ return; }
    if(processKey(MAPEDIT_KEY_2,            MAPEDIT_BIT_2            )){ return; }
    if(processKey(MAPEDIT_KEY_3,            MAPEDIT_BIT_3            )){ return; }
    if(processKey(MAPEDIT_KEY_4,            MAPEDIT_BIT_4            )){ return; }
    if(processKey(MAPEDIT_KEY_5,            MAPEDIT_BIT_5            )){ return; }
    if(processKey(MAPEDIT_KEY_6,            MAPEDIT_BIT_6            )){ return; }
    if(processKey(MAPEDIT_KEY_7,            MAPEDIT_BIT_7            )){ return; }
    if(processKey(MAPEDIT_KEY_8,            MAPEDIT_BIT_8            )){ return; }
    if(processKey(MAPEDIT_KEY_9,            MAPEDIT_BIT_9            )){ return; }
    if(processKey(MAPEDIT_KEY_A,            MAPEDIT_BIT_A            )){ return; }
    if(processKey(MAPEDIT_KEY_B,            MAPEDIT_BIT_B            )){ return; }
    if(processKey(MAPEDIT_KEY_C,            MAPEDIT_BIT_C            )){ return; }
    if(processKey(MAPEDIT_KEY_D,            MAPEDIT_BIT_D            )){ return; }
    if(processKey(MAPEDIT_KEY_E,            MAPEDIT_BIT_E            )){ return; }
    if(processKey(MAPEDIT_KEY_F,            MAPEDIT_BIT_F            )){ return; }
    if(processKey(MAPEDIT_KEY_G,            MAPEDIT_BIT_G            )){ return; }
    if(processKey(MAPEDIT_KEY_H,            MAPEDIT_BIT_H            )){ return; }
    if(processKey(MAPEDIT_KEY_I,            MAPEDIT_BIT_I            )){ return; }
    if(processKey(MAPEDIT_KEY_J,            MAPEDIT_BIT_J            )){ return; }
    if(processKey(MAPEDIT_KEY_K,            MAPEDIT_BIT_K            )){ return; }
    if(processKey(MAPEDIT_KEY_L,            MAPEDIT_BIT_L            )){ return; }
    if(processKey(MAPEDIT_KEY_M,            MAPEDIT_BIT_M            )){ return; }
    if(processKey(MAPEDIT_KEY_N,            MAPEDIT_BIT_N            )){ return; }
    if(processKey(MAPEDIT_KEY_O,            MAPEDIT_BIT_O            )){ return; }
    if(processKey(MAPEDIT_KEY_P,            MAPEDIT_BIT_P            )){ return; }
    if(processKey(MAPEDIT_KEY_Q,            MAPEDIT_BIT_Q            )){ return; }
    if(processKey(MAPEDIT_KEY_R,            MAPEDIT_BIT_R            )){ return; }
    if(processKey(MAPEDIT_KEY_S,            MAPEDIT_BIT_S            )){ return; }
    if(processKey(MAPEDIT_KEY_T,            MAPEDIT_BIT_T            )){ return; }
    if(processKey(MAPEDIT_KEY_U,            MAPEDIT_BIT_U            )){ return; }
    if(processKey(MAPEDIT_KEY_V,            MAPEDIT_BIT_V            )){ return; }
    if(processKey(MAPEDIT_KEY_W,            MAPEDIT_BIT_W            )){ return; }
    if(processKey(MAPEDIT_KEY_X,            MAPEDIT_BIT_X            )){ return; }
    if(processKey(MAPEDIT_KEY_Y,            MAPEDIT_BIT_Y            )){ return; }
    if(processKey(MAPEDIT_KEY_Z,            MAPEDIT_BIT_Z            )){ return; }
    if(processKey(MAPEDIT_KEY_AT,           MAPEDIT_BIT_AT           )){ return; }
    if(processKey(MAPEDIT_KEY_AMPERSAND,    MAPEDIT_BIT_AMPERSAND    )){ return; }
    if(processKey(MAPEDIT_KEY_APOSTROPHE,   MAPEDIT_BIT_APOSTROPHE   )){ return; }
    if(processKey(MAPEDIT_KEY_ASTERISK,     MAPEDIT_BIT_ASTERISK     )){ return; }
    if(processKey(MAPEDIT_KEY_BACKSLASH,    MAPEDIT_BIT_BACKSLASH    )){ return; }
    if(processKey(MAPEDIT_KEY_BAR,          MAPEDIT_BIT_BAR          )){ return; }
    if(processKey(MAPEDIT_KEY_BRACELEFT,    MAPEDIT_BIT_BRACELEFT    )){ return; }
    if(processKey(MAPEDIT_KEY_BRACERIGHT,   MAPEDIT_BIT_BRACERIGHT   )){ return; }
    if(processKey(MAPEDIT_KEY_BRACKETLEFT,  MAPEDIT_BIT_BRACKETLEFT  )){ return; }
    if(processKey(MAPEDIT_KEY_BRACKETRIGHT, MAPEDIT_BIT_BRACKETRIGHT )){ return; }
    if(processKey(MAPEDIT_KEY_CARET,        MAPEDIT_BIT_CARET        )){ return; }
    if(processKey(MAPEDIT_KEY_COLON,        MAPEDIT_BIT_COLON        )){ return; }
    if(processKey(MAPEDIT_KEY_COMMA,        MAPEDIT_BIT_COMMA        )){ return; }
    if(processKey(MAPEDIT_KEY_DOLLAR,       MAPEDIT_BIT_DOLLAR       )){ return; }
    if(processKey(MAPEDIT_KEY_EQUAL,        MAPEDIT_BIT_EQUAL        )){ return; }
    if(processKey(MAPEDIT_KEY_EXCLAMATION,  MAPEDIT_BIT_EXCLAMATION  )){ return; }
    if(processKey(MAPEDIT_KEY_GREATER,      MAPEDIT_BIT_GREATER      )){ return; }
    if(processKey(MAPEDIT_KEY_LESSER,       MAPEDIT_BIT_LESSER       )){ return; }
    if(processKey(MAPEDIT_KEY_MINUS,        MAPEDIT_BIT_MINUS        )){ return; }
    if(processKey(MAPEDIT_KEY_NUMBER,       MAPEDIT_BIT_NUMBER       )){ return; }
    if(processKey(MAPEDIT_KEY_PARENLEFT,    MAPEDIT_BIT_PARENLEFT    )){ return; }
    if(processKey(MAPEDIT_KEY_PARENRIGHT,   MAPEDIT_BIT_PARENRIGHT   )){ return; }
    if(processKey(MAPEDIT_KEY_PERCENT,      MAPEDIT_BIT_PERCENT      )){ return; }
    if(processKey(MAPEDIT_KEY_PERIOD,       MAPEDIT_BIT_PERIOD       )){ return; }
    if(processKey(MAPEDIT_KEY_QUESTION,     MAPEDIT_BIT_QUESTION     )){ return; }
    if(processKey(MAPEDIT_KEY_SEMICOLON,    MAPEDIT_BIT_SEMICOLON    )){ return; }
    if(processKey(MAPEDIT_KEY_SLASH,        MAPEDIT_BIT_SLASH        )){ return; }
    if(processKey(MAPEDIT_KEY_UNDERSCORE,   MAPEDIT_BIT_UNDERSCORE   )){ return; }

#ifdef DEBUG
    fprintf(stderr, "key pressed: %x\n", key);
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
