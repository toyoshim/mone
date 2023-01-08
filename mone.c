// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "ch559.h"
#include "serial.h"
#include "timer3.h"
#include "usb_device.h"

#include "descriptors.h"
#include "settings.h"

void main() {
  initialize();
  settings_init();

  delay(30);

  struct usb_device device;
  device.get_descriptor_size = descriptors_size;
  device.get_descriptor = descriptors_get;
  device.ep_in = descriptors_report;
  usb_device_init(&device, UD_USE_EP1);

  Serial.printf("\nBoot MONE v1.00\n");

  for (;;) {
    if (settings_poll())
      usb_device_init(&device, UD_USE_EP1);
  }
}
