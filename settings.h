// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef __settings_h__
#define __settings_h__

#include <stdbool.h>
#include <stdint.h>

// USB device mode
enum {
  NEOGEO_MINI,
  MEGADRIVE_MINI,
  PC_ENGINE_MINI,
  PLAYSTATION_CLASSIC,
  RETRO_BIT_GENERATIONS,

  NOT_ASSIGNED,
};

// Button mask
enum {
  B_COIN,
  B_START,
  B_UP,
  B_DOWN,
  B_LEFT,
  B_RIGHT,
  B_1,
  B_2,
  B_3,
  B_4,
  B_5,
  B_6,
  B_7,
  B_8,
};

struct setting {
  uint8_t mode;  // USB device mode
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