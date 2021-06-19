// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/usb.h"
#include "chlib/usb_device.h"

#include "descriptors.h"

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
      Serial.println("Get DEVICE Descriptor\n");
      return desc_len_device;
    case USB_DESC_CONFIGURATION:
      return desc_len_configuration;
    case USB_DESC_STRING:
      switch (no) {
        case 0:  // language
          return desc_len_string_0;
        case 1:  // manufacturer
          return desc_len_string_1;
        default:
          return 0;
      }
    case USB_DESC_HID_REPORT:
      return desc_len_hid_report;
    default:
      return 0;
  }
}

const uint8_t* get_descriptor(uint8_t type, uint8_t no) {
  switch (type) {
    case USB_DESC_DEVICE:
      return desc_device;
    case USB_DESC_CONFIGURATION:
      return desc_configuration;
    case USB_DESC_STRING:
      switch (no) {
        case 0:  // language
          return desc_string_0;
        case 1:  // manufacturer
          return desc_string_1;
        default:
          return 0;
      }
    case USB_DESC_HID_REPORT:
      return desc_hid_report;
    default:
      return 0;
  }
}

uint8_t ep1_in(char* buffer) {
  buffer[0] = 0x1b;
  buffer[1] = 0x0c;
  buffer[2] = 0x0f;
  for (uint8_t i = 3; i < 8; ++i)
    buffer[i] = 0x00;
  return 8;
}

void main() {
  initialize();
  delay(30);
  Serial.printf("\nBoot MONE v1.00\n");

  pinMode(1, 5, OUTPUT);
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
      usb_device_init(&device);
    }
    last = next;
  }
}
