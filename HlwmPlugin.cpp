#include <stdint.h>
#include "HlwmPlugin.h"
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

// public:
uint8_t Hlwm::cmdLayer;
cRGB Hlwm::color = CRGB(255, 0, 255);
uint8_t Hlwm::lock_hue = 170;

// private:
KeyAddr Hlwm::cmdLayerToggleKeyAddr;
bool Hlwm::cmdActive = false;

EventHandlerResult Hlwm::onSetup(void) {
  return EventHandlerResult::OK;
}

void Hlwm::setKeyboardLEDColors(void) {
  ::LEDControl.set_mode(::LEDControl.get_mode_index());

  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);
    Key layer_key = Layer.getKey(cmdLayer, key_addr);

    if (k == LockLayer(cmdLayer)) {
      cmdLayerToggleKeyAddr = key_addr;
    }

    if ((k != layer_key) || (k == Key_NoKey) || (k.getFlags() != KEY_FLAGS)) {
      ::LEDControl.refreshAt(KeyAddr(key_addr));
    } else {
      ::LEDControl.setCrgbAt(KeyAddr(key_addr), color);
    }
  }

  if (cmdLayerToggleKeyAddr.isValid()) {
    cRGB lock_color = breath_compute(lock_hue);
    ::LEDControl.setCrgbAt(KeyAddr(cmdLayerToggleKeyAddr), lock_color);
  }
}

EventHandlerResult Hlwm::afterEachCycle() {
  if (!Layer.isActive(cmdLayer)) {
    if (cmdActive) {
      ::LEDControl.set_mode(::LEDControl.get_mode_index());
      cmdActive = false;
    }
  } else {
    if (!cmdActive)  {
      cmdActive = true;
    }
    setKeyboardLEDColors();
  }
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::Hlwm Hlwm;
