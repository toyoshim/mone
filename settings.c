// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "settings.h"

#include "chlib/ch559.h"
#include "chlib/flash.h"
#include "chlib/led.h"

static uint8_t mode = S_NORMAL;
static uint8_t usb_mode = U_NEOGEO_MINI;
static uint16_t poll_tick = 0;
static const uint16_t poll_interval = 17;
static uint8_t last_button = 0;

struct setting settings[16];

static uint8_t dipsw() {
  return ~digitalReadPort(2) & 0x0f;
}

static void slow_poll() {
  poll_tick += poll_interval;
  uint8_t button = digitalRead(4, 6);
  if (last_button & !button) {
    mode = (mode + 1) % S_NOT_ASSIGNED;
    // TODO
  }
  // TODO
  last_button = button;
}

void settings_init() {
  led_init(0, 7, LOW);

  // Setup for a button
  pinMode(4, 6, INPUT_PULLUP);

  // Setup for DIPSW
  for (uint8_t pin = 0; pin <= 3; ++pin)
    pinMode(2, pin, INPUT_PULLUP);

  // Initialize data in flash, and get a copy
  flash_init(*(uint32_t*)"MONE");
  flash_read(4, (uint8_t*)settings, sizeof(settings));
  bool dirty = false;
  for (uint8_t i = 0; i < 16; ++i) {
    if (settings[i].mode != 0xff)
      continue;
    dirty = true;
    settings[i].speed = 17;
    settings[i].rapid_fire = 0;
    settings[i].button_masks[ 0] = (uint16_t)1 << 15;  // Coin
    settings[i].button_masks[ 1] = (uint16_t)1 << 14;  // Start
    settings[i].button_masks[ 2] = (uint16_t)1 << 13;  // Up
    settings[i].button_masks[ 3] = (uint16_t)1 << 12;  // Down
    settings[i].button_masks[ 4] = (uint16_t)1 << 11;  // Left
    settings[i].button_masks[ 5] = (uint16_t)1 << 10;  // Right
    settings[i].button_masks[ 6] = (uint16_t)1 <<  9;  // 1
    settings[i].button_masks[ 7] = (uint16_t)1 <<  8;  // 2
    settings[i].button_masks[ 8] = (uint16_t)1 <<  7;  // 3
    settings[i].button_masks[ 9] = (uint16_t)1 <<  6;  // 4
    settings[i].button_masks[10] = (uint16_t)1 <<  5;  // 5
    settings[i].button_masks[11] = (uint16_t)1 <<  4;  // 6
    settings[i].button_masks[12] = (uint16_t)1 <<  3;  // 7
    settings[i].button_masks[13] = (uint16_t)1 <<  2;  // 8
    for (uint8_t j = 0; j < 8; ++j)
      settings[i].padding[j] = 0;
    settings[i].mode = i & 7;
  }
  if (dirty)
    settings_save();

  usb_mode = dipsw();
  last_button = digitalRead(4, 6);
  poll_tick = timer3_tick_msec();
}

void settings_save() {
  // Store settings to flash
  flash_write(4, (const uint8_t*)settings, sizeof(settings));
}

uint8_t settings_mode() {
  return mode;
}

struct setting* settings_current() {
  return &settings[usb_mode];
}

bool settings_poll() {
  if (!timer3_tick_msec_between(poll_tick, poll_tick + poll_interval))
    slow_poll();

  led_poll();
  uint8_t new_mode = dipsw();
  bool changed = false;
  if (settings[usb_mode].mode != settings[new_mode].mode)
    changed = true;
  usb_mode = new_mode;
  return changed;
}