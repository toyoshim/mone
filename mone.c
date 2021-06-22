// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/usb.h"
#include "chlib/usb_device.h"

#include "descriptors.h"
#include "led.h"

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

#define BUTTON_REVERSE_ROTATION

static uint8_t mode = 1;
static uint8_t led = 0;
static uint16_t button_masks[12] = {
  (uint16_t)1 << 15,  // Coin
  (uint16_t)1 << 14,  // Start
  (uint16_t)1 << 13,  // Up
  (uint16_t)1 << 12,  // Down
  (uint16_t)1 << 11,  // Left
  (uint16_t)1 << 10,  // Right
#ifndef BUTTON_REVERSE_ROTATION
  (uint16_t)1 <<  9,  // 1
#endif  // BUTTON_REVERRSE_ROTATION
  (uint16_t)1 <<  8,  // 2
  (uint16_t)1 <<  7,  // 3
  (uint16_t)1 <<  6,  // 4
#ifdef BUTTON_REVERSE_ROTATION
  (uint16_t)1 <<  9,  // 1
#endif  // BUTTON_REVERRSE_ROTATION
  (uint16_t)1 <<  5,  // 5
  (uint16_t)1 <<  4,  // 6
};

static void dump(const char* message, const uint8_t* buffer, uint8_t size) {
  Serial.printf("=== %s ===\n", message);
  for (uint8_t i = 0; i < size; ++i) {
    Serial.printc(buffer[i], HEX);
    if (i % 16 == 15)
      Serial.println("");
    else
      Serial.print(", ");
  }
  if (size % 16)
    Serial.println("");
  Serial.println("======");
}

uint8_t get_descriptor_size(uint8_t type, uint8_t no) {
  switch (type) {
    case USB_DESC_DEVICE:
      return desc_len_device[mode];
    case USB_DESC_CONFIGURATION:
      return desc_len_configuration[mode];
    case USB_DESC_STRING:
      switch (no) {
        case 0:  // language
          return desc_len_string_0[mode];
        case 1:
          return desc_len_string_1[mode];
        case 2:
          return desc_len_string_2[mode];
        default:
          return 0;
      }
    case USB_DESC_HID_REPORT:
      return desc_len_hid_report[mode];
    default:
      return 0;
  }
}

const uint8_t* get_descriptor(uint8_t type, uint8_t no) {
  switch (type) {
    case USB_DESC_DEVICE:
      return desc_device[mode];
    case USB_DESC_CONFIGURATION:
      return desc_configuration[mode];
    case USB_DESC_STRING:
      switch (no) {
        case 0:  // language
          return desc_string_0[mode];
        case 1:
          return desc_string_1[mode];
        case 2:
          return desc_string_2[mode];
        default:
          return 0;
      }
    case USB_DESC_HID_REPORT:
      return desc_hid_report[mode];
    default:
      return 0;
  }
}

uint16_t buttons() {                // 7654 3210 bit
  uint8_t p3 = digitalReadPort(3);  // LR12 34S6
  uint8_t p4 = digitalReadPort(4);  // --5C DU--
  uint16_t buttons =  // CSUD LP12 3456 ----
      ((p4 & 0x10) << 11) |  // Coin
      ((p3 & 0x02) << 13) |  // Start
      ((p4 & 0x04) << 11) |  // Up
      ((p4 & 0x08) <<  9) |  // Down
      ((p3 & 0xfd) <<  4) |  // Left, Right, 1, 2, 3, 4, 6
       (p4 & 0x20);          // 5
  return ~buttons;
}

uint8_t ngm_in(char* buffer) {
  uint16_t b = buttons();
  buffer[0] =
      ((b & button_masks[B_3    ]) ? 0x10 : 0) |
      ((b & button_masks[B_2    ]) ? 0x08 : 0) |
      ((b & button_masks[B_1    ]) ? 0x02 : 0) |
      ((b & button_masks[B_4    ]) ? 0x01 : 0);
  buffer[1] =
      ((b & button_masks[B_START]) ? 0x08 : 0) |
      ((b & button_masks[B_COIN ]) ? 0x04 : 0);
  uint8_t stick =
      ((b & button_masks[B_UP   ]) ? 0x01 : 0) |
      ((b & button_masks[B_DOWN ]) ? 0x02 : 0) |
      ((b & button_masks[B_LEFT ]) ? 0x04 : 0) |
      ((b & button_masks[B_RIGHT]) ? 0x08 : 0);
  switch (stick) {
    case 0x01:  // up
      buffer[2] = 0;
      break;
    case 0x02:  // down
      buffer[2] = 4;
      break;
    case 0x04:  // left
      buffer[2] = 6;
      break;
    case 0x05:  // up-left
      buffer[2] = 7;
      break;
    case 0x06:  // down-left
      buffer[2] = 5;
      break;
    case 0x08:  // right
      buffer[2] = 2;
      break;
    case 0x09:  // up-right
      buffer[2] = 1;
      break;
    case 0x0a:  // down-right
      buffer[2] = 3;
      break;
    default:
      buffer[2] = 15;
      break;
  }
  for (uint8_t i = 3; i < 8; ++i)
    buffer[i] = 0;
  return 8;
}

uint8_t mdm_in(char* buffer) {
  uint8_t x = 127;
  uint8_t y = 127;
  uint16_t b = buttons();
  if (b & button_masks[B_LEFT])
    x = 0;
  else if (b & button_masks[B_RIGHT])
    x = 255;
  if (b & button_masks[B_UP])
    y = 0;
  else if (b & button_masks[B_DOWN])
    y = 255;
  buffer[0] = 0;
  buffer[1] = 0;
  buffer[2] = 0;
  buffer[3] = x;
  buffer[4] = y;
  buffer[5] =
      ((b & button_masks[B_4]) ? 0x80 : 0) |  // X(3)
      ((b & button_masks[B_1]) ? 0x40 : 0) |  // A(2)
      ((b & button_masks[B_2]) ? 0x20 : 0) |  // B(1)
      ((b & button_masks[B_5]) ? 0x10 : 0);   // Y(0)
  buffer[6] =
      ((b & button_masks[B_START]) ? 0x20 : 0) |  // 9
      ((b & button_masks[B_COIN]) ? 0x10 : 0) |  // 8
      ((b & button_masks[B_3]) ? 0x02 : 0) |  // C(5)
      ((b & button_masks[B_6]) ? 0x01 : 0);   // Z(4)
  return 8;
}

uint8_t ep1_in(char* buffer) {
  switch (mode) {
    case 0:
      return ngm_in(buffer);
    case 1:
      return mdm_in(buffer);
    default:
      return 0;
  }
}

uint8_t dipsw() {
  return ~digitalReadPort(2) & 0x0f;
}

void main() {
  initialize();

  led_init(0, 7, LOW);

  // Setup for Button
  pinMode(4, 6, INPUT_PULLUP);

  // Setup for DIPSW
  for (uint8_t pin = 0; pin <= 3; ++pin)
    pinMode(2, pin, INPUT_PULLUP);

  // Setup for Controller
  for (uint8_t pin = 0; pin <= 7; ++pin)
    pinMode(3, pin, INPUT_PULLUP);
  for (uint8_t pin = 2; pin <= 5; ++pin)
    pinMode(4, pin, INPUT_PULLUP);

  delay(30);
  Serial.printf("\nBoot MONE v1.00\n");

  pinMode(4, 6, INPUT_PULLUP);

  struct usb_device device;
  device.get_descriptor_size = get_descriptor_size;
  device.get_descriptor = get_descriptor;
  device.ep1_in = ep1_in;
  usb_device_init(&device);

  uint8_t last = digitalRead(4, 6);
  for (;;) {
    uint8_t next = digitalRead(4, 6);
    if (last == HIGH && next == LOW) {
      led++;
      led_oneshot(led);
      Serial.printf("led: %d\n", led);
    }
    last = next;
    led_update();
  }
}
