// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/led.h"
#include "chlib/usb.h"
#include "chlib/usb_device.h"

#include "controller.h"
#include "descriptors.h"
#include "settings.h"

static uint8_t led = 0;

static void dump(const char* message, const uint8_t* buffer, uint16_t size) {
  Serial.printf("=== %s ===\n", message);
  for (uint16_t i = 0; i < size; ++i) {
    Serial.printc(buffer[i], HEX);
    if (i % 32 == 31)
      Serial.println("");
    else
      Serial.print(", ");
  }
  if (size % 32)
    Serial.println("");
  Serial.println("======");
}

uint8_t get_descriptor_size(uint8_t type, uint8_t no) {
  uint8_t mode = settings_current()->mode;
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
  uint8_t mode = settings_current()->mode;
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

uint8_t ep1_in(uint8_t* buffer) {
  return get_report(
      settings_current()->mode,
      controller_get(),
      settings_current()->button_masks,
      buffer);
}

void main() {
  initialize();
  led_init(0, 7, LOW);
  settings_init();

  // Setup for Button
  pinMode(4, 6, INPUT_PULLUP);

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

    if (settings_poll())
      usb_device_init(&device);
  }
}
