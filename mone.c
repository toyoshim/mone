// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/led.h"
#include "chlib/usb_device.h"
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
  device.get_descriptor_size = descriptors_size;
  device.get_descriptor = descriptors_get;
  device.ep1_in = descriptors_report;
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
