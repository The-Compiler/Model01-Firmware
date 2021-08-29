/* Kaleidoscope-Hlwm - A plugin to show herbstluftm tag status.
 * Copyright (C) 2021 Florian Bruhin <me@the-compiler.org>.
 *
 * Heavily based on:
 * Kaleidoscope-NumPad - A NumPad plugin for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {

class Hlwm : public kaleidoscope::Plugin {
 public:
  Hlwm(void) {}

  static cRGB color;
  EventHandlerResult onFocusEvent(const char *command);
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult onSetup(void);

 private:
  void setTagColor(uint8_t tag);
};
}
}

extern kaleidoscope::plugin::Hlwm Hlwm;
