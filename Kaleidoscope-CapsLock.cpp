// Originally based on:
//https://github.com/donfeduardo/Kaleidoscope-CapsLock
// with fixes by @cxw42:
// https://github.com/donfeduardo/Kaleidoscope-CapsLock/pull/5

#include "Kaleidoscope-CapsLock.h"
#include "Kaleidoscope.h"

bool CapsLock_::capsCleanupDone = true;
uint8_t CapsLock_::shiftHue = 170;

static bool getCapsLockState() {
  return !!(kaleidoscope::Runtime.hid().keyboard().getKeyboardLEDs() & LED_CAPS_LOCK);
}

/*
  this will paint the preferred color onto letter keys as long as capsState is true.
  When capsState is set to false, reset the color mode to its previous state.
*/
kaleidoscope::EventHandlerResult CapsLock_::afterEachCycle() {
  bool capsState = getCapsLockState();
  if (capsState) {
    capsCleanupDone = false;
    for (uint8_t r = 0; r < kaleidoscope::Device::KeyScannerProps::matrix_rows; r++) {
      for (uint8_t c = 0; c < kaleidoscope::Device::KeyScannerProps::matrix_columns; c++) {
        Key k = Layer.lookupOnActiveLayer(KeyAddr(r, c));

        cRGB shiftColor = breath_compute(shiftHue);

        if (highlightShiftKeys && (k == Key_LeftShift || k == Key_RightShift)) {
          ::LEDControl.setCrgbAt(KeyAddr(r, c), shiftColor);
        } else {
          ::LEDControl.refreshAt(KeyAddr(r, c));
        }
      }
    }
    return kaleidoscope::EventHandlerResult::OK;
  } else {
    if (!capsCleanupDone) {
      // Some keys seem to get "stuck" in the painted color. Reset current mode to unstick them.
      ::LEDControl.set_mode(::LEDControl.get_mode_index());
      capsCleanupDone = true;
    }
    return kaleidoscope::EventHandlerResult::OK;
  }
}

CapsLock_ CapsLock;
