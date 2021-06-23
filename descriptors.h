// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef __descriptors_h__
#define __descriptors_h__

#include <stdint.h>

#include "chlib/usb.h"

enum {
  NEOGEO_MINI = 0,
  MEGADRIVE_MINI = 1,
  PC_ENGINE_MINI = 2,
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
};

// modes:
//  0 - NEOGEO mini
//  1 - MEGADRIVE mini / Astrocity mini
//  2 - PC-Engine mini
//  3 - PlayStation classic
//  4 - Retro-Bit Generations
extern const uint8_t* desc_device[];
extern const uint8_t* desc_configuration[];
extern const uint8_t* desc_string_0[];
extern const uint8_t* desc_string_1[];
extern const uint8_t* desc_string_2[];
extern const uint8_t* desc_hid_report[];

extern const uint8_t desc_len_device[];
extern const uint8_t desc_len_configuration[];
extern const uint8_t desc_len_string_0[];
extern const uint8_t desc_len_string_1[];
extern const uint8_t desc_len_string_2[];
extern const uint8_t desc_len_hid_report[];

uint8_t get_report(
    uint8_t mode, uint16_t buttons, uint16_t* button_masks, uint8_t* bufferr);

#endif