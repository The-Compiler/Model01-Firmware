#include <stdint.h>
#include "HlwmPlugin.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

//cRGB Hlwm::color = CRGB(255, 0, 255);

static Key tag_keys[] = {Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9, Key_0};

void Hlwm::setTagColor(KeyAddr keyAddr, uint8_t tagIndex) {
    cRGB color = CRGB(tagIndex * 25, 0, 0);
    ::LEDControl.setCrgbAt(KeyAddr(keyAddr), color);
}

EventHandlerResult Hlwm::onKeyswitchEvent(Key &mappedKey, KeyAddr keyAddr, uint8_t keyState) {
    if (mappedKey != Key_LeftGui) {
        return EventHandlerResult::OK;
    }
    
    if (keyIsPressed(keyState)) {
        for (auto key_addr : KeyAddr::all()) {
            Key k = Layer.lookupOnActiveLayer(key_addr);
            for (int i = 0; i < sizeof(tag_keys) / sizeof(tag_keys[0]); i++) {
                if (k == tag_keys[i]) {
                    setTagColor(key_addr, i);
                }
            }
        }
    };

    return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::Hlwm Hlwm;
