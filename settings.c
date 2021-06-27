// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "settings.h"

#include "chlib/ch559.h"
#include "chlib/flash.h"
#include "chlib/led.h"
#include "controller.h"

static uint8_t mode = S_NORMAL;
static uint8_t mode_step = 0;
static uint16_t mode_data = 0;
static uint8_t mode_dipsw = 0;
static uint8_t usb_mode = U_NEOGEO_MINI;
static uint16_t poll_tick = 0;
static const uint16_t poll_interval = 17;
static uint8_t last_button = 0;

struct setting settings[16];

static uint8_t dipsw() {
  return ~digitalReadPort(2) & 0x0f;
}

static void mode_layout() {
  uint16_t buttons = controller_get() & 0x03ff;
  if (mode_data && !buttons) {
    if (mode_step < 8) {
      settings[mode_dipsw].button_masks[B_1 + mode_step] = mode_data;
      led_mode(L_FASTER_BLINK);
    }
    mode_data = 0;
    mode_step++;
  }
  mode_data |= buttons;
  if (mode_data)
    led_mode(L_OFF);
}

static void quit_layout() {
  for (uint8_t i = mode_step; i < 8; ++i)
    settings[mode_dipsw].button_masks[B_1 + i] = 0;
}

static void mode_rapid() {
  uint16_t buttons = controller_get() & 0x3ff;
  if (mode_data && !buttons) {
    settings[mode_dipsw].rapid_fire = mode_data;
    led_mode(L_FAST_BLINK);
    mode_data = 0;
  }
  mode_data |= buttons;
  if (mode_data)
    led_mode(L_OFF);
}

static void mode_speed() {
  uint16_t buttons = controller_get() & 0x3ff;
  if (buttons & 0x0040) {
    settings[mode_dipsw].speed = 17;  // 30
  } else if (buttons & 0x0080) {
    settings[mode_dipsw].speed = 25;  // 20
  } else if (buttons & 0x0100) {
    settings[mode_dipsw].speed = 33;  // 15
  } else if (buttons & 0x0200) {
    settings[mode_dipsw].speed = 42;  // 12
  }
  led_mode((buttons & 0x03c0) ? L_OFF : L_BLINK);
}

static void quit_speed() {
  settings_save();
}

static void slow_poll() {
  poll_tick += poll_interval;
  uint8_t button = digitalRead(4, 6);
  if (last_button & !button) {
    switch (mode) {
      case S_LAYOUT:
        quit_layout();
      case S_SPEED:
        quit_speed();
        break;
    }
    mode = (mode + 1) % S_NOT_ASSIGNED;
    mode_step = 0;
    mode_data = 0;
    mode_dipsw = dipsw();
    switch (mode) {
      case S_NORMAL:
        led_mode(L_ON);
        break;
      case S_LAYOUT:
        led_mode(L_FASTER_BLINK);
        break;
      case S_RAPID:
        led_mode(L_FAST_BLINK);
        break;
      case S_SPEED:
        led_mode(L_BLINK);
        break;
    }
  }
  switch (mode) {
    case S_LAYOUT:
      mode_layout();
      break;
    case S_RAPID:
      mode_rapid();
      break;
    case S_SPEED:
      mode_speed();
      break;
  }
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