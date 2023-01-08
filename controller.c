// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "controller.h"

#include "ch559.h"

void controller_init() {
  for (uint8_t pin = 0; pin <= 7; ++pin)
    pinMode(3, pin, INPUT_PULLUP);
  for (uint8_t pin = 2; pin <= 5; ++pin)
    pinMode(4, pin, INPUT_PULLUP);
}

uint16_t controller_get() {         // 7654 3210 bit
  uint8_t p3 = digitalReadPort(3);  // LR12 34S6
  uint8_t p4 = digitalReadPort(4);  // --5C DU--
  uint16_t buttons = 0x0f |  // CSUD LP12 3456 ----
      ((p4 & 0x10) << 11) |  // Coin
      ((p3 & 0x02) << 13) |  // Start
      ((p4 & 0x04) << 11) |  // Up
      ((p4 & 0x08) <<  9) |  // Down
      ((p3 & 0xfd) <<  4) |  // Left, Right, 1, 2, 3, 4, 6
       (p4 & 0x20);          // 5
  return ~buttons;
}
