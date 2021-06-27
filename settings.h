// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef __settings_h__
#define __settings_h__

#include <stdbool.h>
#include <stdint.h>

struct setting {
  uint8_t mode;  // USB device mode, see descriptors.h
  uint8_t speed;  // Rapid fire speed (flip per every N msec)
  uint16_t rapid_fire;
  uint16_t button_masks[14];
  uint16_t padding[8];
};

extern struct setting settings[16];

void settings_init();
void settings_save();

struct setting* settings_current();

// Returns true if USB device reset is needed.
bool settings_poll();

#endif  // __settings_h__
