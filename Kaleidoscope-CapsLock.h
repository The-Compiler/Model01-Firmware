#pragma once

#include "Kaleidoscope-LEDControl.h"

class CapsLock_ : public kaleidoscope::Plugin {
  public:
    CapsLock_(void) {}

    kaleidoscope::EventHandlerResult afterEachCycle();

  private:
    static bool capsCleanupDone;
};

extern CapsLock_ CapsLock;
