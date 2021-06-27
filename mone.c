// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/usb_device.h"
#include "descriptors.h"
#include "settings.h"

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

void main() {
  initialize();
  settings_init();

  delay(30);

  struct usb_device device;
  device.get_descriptor_size = descriptors_size;
  device.get_descriptor = descriptors_get;
  device.ep1_in = descriptors_report;
  usb_device_init(&device);

  Serial.printf("\nBoot MONE v1.00\n");

  for (;;) {
    if (settings_poll())
      usb_device_init(&device);
  }
}
