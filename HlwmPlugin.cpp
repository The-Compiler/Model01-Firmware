#include <stdint.h>
#include "HlwmPlugin.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

//cRGB Hlwm::color = CRGB(255, 0, 255);

#define TAG_COUNT (sizeof(tagKeys) / sizeof(Key))
static Key tagKeys[] = {Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9, Key_0};
static KeyAddr tagKeyAddrs[TAG_COUNT];

void Hlwm::setTagColor(uint8_t tagIndex) {
    KeyAddr keyAddr = tagKeyAddrs[tagIndex];
    cRGB color = CRGB(tagIndex * 25, 0, 0);
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

}
}

kaleidoscope::plugin::Hlwm Hlwm;
