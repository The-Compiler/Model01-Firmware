#include <stdint.h>
#include "Kaleidoscope.h"
#include "HlwmPlugin.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/plugin/LED-Palette-Theme.h"
#include "Kaleidoscope-FocusSerial.h"

namespace kaleidoscope {
namespace plugin {

#define TAG_COUNT (sizeof(tagKeys) / sizeof(Key))
static Key tagKeys[] = {Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9, Key_0};
static KeyAddr tagKeyAddrs[TAG_COUNT];
static char tagStatus[TAG_COUNT] = {'.', ':', '.', ':', '.', ':', '.', ':', '.', '!'};

#define TAG_CHAR_EMPTY '.'
#define TAG_CHAR_USED ':'
#define TAG_CHAR_FOCUSED '#'
#define TAG_CHAR_UNFOCUSED '-'
#define TAG_CHAR_URGENT '!'
// we should never see + and % if we use tag_status without specifying a monitor

#define COLOR_IDX_EMPTY 0
#define COLOR_IDX_USED 1
#define COLOR_IDX_FOCUSED 2
#define COLOR_IDX_UNFOCUSED 3
#define COLOR_IDX_URGENT 4
#define COLOR_IDX_UNKNOWN 5

#define FOCUS_COMMAND "hlwm.tagstatus"
#define FOCUS_COMMAND_LEN (sizeof(FOCUS_COMMAND) - 1)

void Hlwm::setTagColor(uint8_t tagIndex) {
    KeyAddr keyAddr = tagKeyAddrs[tagIndex];
    uint8_t colorIdx = COLOR_IDX_UNKNOWN;

    switch (tagStatus[tagIndex]) {
        case TAG_CHAR_EMPTY:
            colorIdx = COLOR_IDX_EMPTY;
        case TAG_CHAR_USED:
            colorIdx = COLOR_IDX_USED;
        case TAG_CHAR_FOCUSED:
            colorIdx = COLOR_IDX_FOCUSED;
        case TAG_CHAR_UNFOCUSED:
            colorIdx = COLOR_IDX_UNFOCUSED;
        case TAG_CHAR_URGENT:
            colorIdx = COLOR_IDX_URGENT;
    }

    cRGB color = LEDPaletteTheme::lookupPaletteColor(colorIdx);
    ::LEDControl.setCrgbAt(KeyAddr(keyAddr), color);
}

EventHandlerResult Hlwm::onSetup(void) {
    for (int i = 0; i < TAG_COUNT; i++) {
        for (auto keyAddr : KeyAddr::all()) {
            Key k = Layer.lookupOnActiveLayer(keyAddr);
            if (k == tagKeys[i]) {
                tagKeyAddrs[i] = keyAddr;
            }
        }
    }
    //memset(tagStatus, '?', TAG_COUNT);
    return EventHandlerResult::OK;
}

EventHandlerResult Hlwm::onKeyswitchEvent(Key &mappedKey, KeyAddr keyAddr, uint8_t keyState) {
    if (mappedKey != Key_LeftGui) {
        return EventHandlerResult::OK;
    }
    
    if (keyIsPressed(keyState)) {
        for (int i = 0; i < TAG_COUNT; i++) {
            setTagColor(i);
        }
    } else if (keyToggledOff(keyState)) {
        for (int i = 0; i < TAG_COUNT; i++) {
            ::LEDControl.refreshAt(tagKeyAddrs[i]);
        }
    }

    return EventHandlerResult::OK;
}

EventHandlerResult Hlwm::onFocusEvent(const char *command) {
    if (::Focus.handleHelp(command, PSTR(FOCUS_COMMAND))) {
        return EventHandlerResult::OK;
    }

    if (strncmp_P(command, PSTR(FOCUS_COMMAND), FOCUS_COMMAND_LEN) != 0) {
        return EventHandlerResult::OK;
    }

    for (uint8_t i = 0; i < TAG_COUNT; i++) {
        tagStatus[i] = Kaleidoscope.serialPort().read();
    }
    return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::Hlwm Hlwm;
