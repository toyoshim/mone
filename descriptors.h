// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef __descriptors_h__
#define __descriptors_h__

#include <stdint.h>

#include "chlib/usb.h"

// USB device mode
enum {
  NEOGEO_MINI = 0,
  MEGADRIVE_MINI = 1,
  PC_ENGINE_MINI = 2,
  PLAYSTATION_CLASSIC = 3,
  RETRO_BIT_GENERATIONS = 4,

  NOT_ASSIGNED = 5,
};

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

uint8_t descriptors_size(uint8_t type, uint8_t no);
const uint8_t* descriptors_get(uint8_t type, uint8_t no);
uint8_t descriptors_report(uint8_t* buffer);

#endif