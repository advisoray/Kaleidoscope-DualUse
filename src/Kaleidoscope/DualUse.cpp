/* -*- mode: c++ -*-
 * Kaleidoscope-DualUse -- Dual use keys for Kaleidoscope
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-DualUse.h>
#include <kaleidoscope/hid.h>

namespace kaleidoscope {
uint16_t DualUse::key_action_needed_map_;
uint16_t DualUse::pressed_map_;
uint32_t DualUse::end_time_;
uint16_t DualUse::time_out = 1000;

// ---- helpers ----
Key DualUse::specialAction(uint8_t spec_index) {
  Key new_key;

  new_key.flags = KEY_FLAGS;
  if (spec_index < 8) {
    new_key.keyCode = Key_LeftControl.keyCode + spec_index;
  } else {
    uint8_t target = spec_index - 8;

    Layer.on(target);

    new_key.keyCode = 0;
  }

  return new_key;
}

void DualUse::pressAllSpecials(byte row, byte col) {
  for (uint8_t spec_index = 0; spec_index < 32; spec_index++) {
    if (!bitRead(pressed_map_, spec_index))
      continue;

    Key new_key = specialAction(spec_index);
    if (new_key.raw != Key_NoKey.raw)
      handleKeyswitchEvent(new_key, row, col, IS_PRESSED | INJECTED);
  }
}

// ---- API ----

DualUse::DualUse(void) {
}

void DualUse::begin(void) {
  Kaleidoscope.useEventHandlerHook(eventHandlerHook);
}

void DualUse::inject(Key key, uint8_t key_state) {
  eventHandlerHook(key, UNKNOWN_KEYSWITCH_LOCATION, key_state);
}

// ---- Handlers ----

Key DualUse::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & INJECTED)
    return mapped_key;

  // If nothing happened, bail out fast.
  if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
    if (mapped_key.raw < ranges::DU_FIRST || mapped_key.raw > ranges::DU_LAST)
      return mapped_key;
    return Key_NoKey;
  }

  if (mapped_key.raw >= ranges::DU_FIRST && mapped_key.raw <= ranges::DU_LAST) {
    uint8_t spec_index = (mapped_key.raw - ranges::DU_FIRST) >> 8;
    Key new_key = Key_NoKey;

    if (keyToggledOn(key_state)) {
      bitWrite(pressed_map_, spec_index, 1);
      bitWrite(key_action_needed_map_, spec_index, 1);
      end_time_ = millis() + time_out;
    } else if (keyIsPressed(key_state)) {
      if (millis() >= end_time_) {
        new_key = specialAction(spec_index);
        bitWrite(key_action_needed_map_, spec_index, 0);
      }
    } else if (keyToggledOff(key_state)) {
      if ((millis() < end_time_) && bitRead(key_action_needed_map_, spec_index)) {
        uint8_t m = mapped_key.raw - ranges::DU_FIRST - (spec_index << 8);
        if (spec_index >= 8)
          m--;

        Key new_key = { m, KEY_FLAGS };

        handleKeyswitchEvent(new_key, row, col, IS_PRESSED | INJECTED);
        hid::sendKeyboardReport();
      } else {
        if (spec_index >= 8) {
          uint8_t target = spec_index - 8;

          Layer.off(target);
        }
      }

      bitWrite(pressed_map_, spec_index, 0);
      bitWrite(key_action_needed_map_, spec_index, 0);
    }

    return new_key;
  }

  if (pressed_map_ == 0) {
    return mapped_key;
  }

  pressAllSpecials(row, col);
  key_action_needed_map_ = 0;

  if (pressed_map_ > (1 << 7)) {
    mapped_key = Layer.lookup(row, col);
  }

  return mapped_key;
}

}

kaleidoscope::DualUse DualUse;
