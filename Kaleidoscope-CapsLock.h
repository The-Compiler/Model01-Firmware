#pragma once

#include "Kaleidoscope-LEDControl.h"

class CapsLock_ : public kaleidoscope::Plugin {
  public:
    CapsLock_(void) {}
    static uint8_t shiftHue;
    static uint8_t highlightShiftKeys;

    kaleidoscope::EventHandlerResult afterEachCycle();

  private:
    static bool capsCleanupDone;
};

extern CapsLock_ CapsLock;
