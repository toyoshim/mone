// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "chlib/ch559.h"
#include "chlib/timer3.h"
#include "chlib/usb_device.h"
#include "descriptors.h"
#include "settings.h"

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
