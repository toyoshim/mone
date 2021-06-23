// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include "descriptors.h"

#include "chlib/usb.h"

static const uint8_t ngm_device[] = {
  0x12,  // size
  USB_DESC_DEVICE,
  0x10, 0x01,  // USB version
  0x00,  // device class
  0x00,  // device subclass
  0x00,  // protocol
  0x40,  // max packet size
  0xbc, 0x20,  // vendor ID
  0x00, 0x55,  // device ID
  0x00, 0x01,  // device version
  0x01,  // manufacturer string index
  0x00,  // product string index
  0x00,  // serial number string index
  0x01,  // number of configurations
};

static const uint8_t mdm_device[] = {
  0x12,  // size
  USB_DESC_DEVICE,
  0x00, 0x02,  // USB version
  0x00,  // device class
  0x00,  // device subclass
  0x00,  // protocol
  0x40,  // max packet size
  0xa3, 0x0c,  // vendor ID
  0x24, 0x00,  // device ID
  0x06, 0x02,  // device version
  0x00,  // manufacturer string index
  0x02,  // product string index
  0x00,  // serial number string index
  0x01,  // number of configurations
};

static const uint8_t pem_device[] = {
  0x12,  // size
  USB_DESC_DEVICE,
  0x00, 0x02,  // USB version
  0x00,  // device class
  0x00,  // device subclass
  0x00,  // protocol
  0x40,  // max packet size
  0x0d, 0x0f,  // vendor ID
  0x38, 0x01,  // device ID
  0x08, 0x01,  // device version
  0x01,  // manufacturer string index
  0x02,  // product string index
  0x00,  // serial number string index
  0x01,  // number of configurations
};

static const uint8_t psc_device[] = {
  0x12,  // size
  USB_DESC_DEVICE,
  0x00, 0x02,  // USB version
  0x00,  // device class
  0x00,  // device subclass
  0x00,  // protocol
  0x40,  // max packet size
  0x4c, 0x05,  // vendor ID
  0xda, 0x0c,  // device ID
  0x00, 0x01,  // device version
  0x01,  // manufacturer string index
  0x02,  // product string index
  0x00,  // serial number string index
  0x01,  // number of configurations
};

static const uint8_t ngm_configuration[] = {
  0x09,  // size
  USB_DESC_CONFIGURATION,
  0x22, 0x00,  // total length
  0x01,  // number of interfaces
  0x01,  // index of this configuration
  0x00,  // configuration name string index
  0xa0,  // attributes
  0x32, // 100mA

  // interface descriptor
  0x09,  // size
  USB_DESC_INTERFACE,
  0x00,  // index of this interface
  0x00,  // alternate setting for this interface
  0x01,  // number of endpoints
  0x03,  // interface class (HID)
  0x00,  // interface subclass
  0x00,  // interface protocol
  0x00,  // string index for interface

  // hid report descriptor
  0x09,  // size
  USB_DESC_HID,
  0x01, 0x01,  // BCD representation of HID verrsion
  0x00,  // target country code
  0x01,  // number of HID report
  USB_DESC_HID_REPORT,
  0x61, 0x00,  // descriptor length

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x81,  // IN endpoint number 1
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x0a,  // poll interval 10ms
};

static const uint8_t mdm_configuration[] = {
  0x09,  // size
  USB_DESC_CONFIGURATION,
  0x29, 0x00,  // total length
  0x01,  // number of interfaces
  0x01,  // index of this configuration
  0x00,  // configuration name string index
  0x80,  // attributes
  0x32,  // 100mA

  // interface descriptor
  0x09,  // size
  USB_DESC_INTERFACE,
  0x00,  // index of this interface
  0x00,  // alternate setting for this interface
  0x02,  // number of endpoints
  0x03,  // interface class (HID)
  0x00,  // interface subclass
  0x00,  // interface protocol
  0x00,  // string index for interface

  // hid report descriptor
  0x09,  // size
  USB_DESC_HID,
  0x11, 0x01,  // BCD representation of HID verrsion
  0x00,  // target country code
  0x01,  // number of HID report
  USB_DESC_HID_REPORT,
  0x65, 0x00,  // descriptor length

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x02,  // OUT endpoint number 2
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x0a,  // poll interval 10ms

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x81,  // IN endpoint number 1
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x0a,  // poll interval 10ms
};

static const uint8_t pem_configuration[] = {
  0x09,  // size
  USB_DESC_CONFIGURATION,
  0x29, 0x00,  // total length
  0x01,  // number of interfaces
  0x01,  // index of this configuration
  0x00,  // configuration name string index
  0x80,  // attributes
  0x32,  // 100mA

  // interface descriptor
  0x09,  // size
  USB_DESC_INTERFACE,
  0x00,  // index of this interface
  0x00,  // alternate setting for this interface
  0x02,  // number of endpoints
  0x03,  // interface class (HID)
  0x00,  // interface subclass
  0x00,  // interface protocol
  0x00,  // string index for interface

  // hid report descriptor
  0x09,  // size
  USB_DESC_HID,
  0x11, 0x01,  // BCD representation of HID verrsion
  0x00,  // target country code
  0x01,  // number of HID report
  USB_DESC_HID_REPORT,
  0x5e, 0x00,  // descriptor length

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x02,  // OUT endpoint number 2
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x05,  // poll interval 5ms

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x81,  // IN endpoint number 1
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x05,  // poll interval 5ms
};

static const uint8_t psc_configuration[] = {
  0x09,  // size
  USB_DESC_CONFIGURATION,
  0x22, 0x00,  // total length
  0x01,  // number of interfaces
  0x01,  // index of this configuration
  0x00,  // configuration name string index
  0xa0,  // attributes
  0x32,  // 100mA

  // interface descriptor
  0x09,  // size
  USB_DESC_INTERFACE,
  0x00,  // index of this interface
  0x00,  // alternate setting for this interface
  0x01,  // number of endpoints
  0x03,  // interface class (HID)
  0x00,  // interface subclass
  0x00,  // interface protocol
  0x00,  // string index for interface

  // hid report descriptor
  0x09,  // size
  USB_DESC_HID,
  0x11, 0x01,  // BCD representation of HID verrsion
  0x00,  // target country code
  0x01,  // number of HID report
  USB_DESC_HID_REPORT,
  0x31, 0x00,  // descriptor length

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x81,  // IN endpoint number 1
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x0a,  // poll interval 10ms
};


static const uint8_t string_0[] = { 0x04, 0x03, 0x09, 0x04 };  // language descriptor

static const uint8_t ngm_string_1[] = { 0x06, 0x03, 'J', 0, 'J', 0 };

static const uint8_t mdm_string_2[] = {
  0x1c, 0x03,
  '6', 0, 'B', 0, ' ', 0, 'c', 0, 'o', 0, 'n', 0, 't', 0, 'r', 0,
  'o', 0, 'l', 0, 'l', 0, 'e', 0, 'r', 0,
};

static const uint8_t pem_string_1[] = {
  0x1c, 0x03,
  'H', 0, 'O', 0, 'R', 0, 'I', 0, ' ', 0, 'C', 0, 'O', 0, '.', 0,
  ',', 0, 'L', 0, 'T', 0, 'D', 0, '.', 0,
};

static const uint8_t pem_string_2[] = {
  0x1a, 0x03,
  'P', 0, 'C', 0, 'E', 0, 'n', 0, 'g', 0, 'i', 0, 'n', 0, 'e', 0,
  ' ', 0, 'P', 0, 'A', 0, 'D', 0,
};

static const uint8_t psc_string_1[] = {
  0x3e, 0x03,
  'S', 0, 'o', 0, 'n', 0, 'y', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0,
  'e', 0, 'r', 0, 'a', 0, 'c', 0, 't', 0, 'i', 0, 'v', 0, 'e', 0,
  ' ', 0, 'E', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 't', 0, 'a', 0,
  'i', 0, 'n', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0,
};

static const uint8_t psc_string_2[] = {
  0x16, 0x03,
  'C', 0, 'o', 0, 'n', 0, 't', 0, 'r', 0, 'o', 0, 'l', 0, 'l', 0,
  'e', 0, 'r', 0,
};

static const uint8_t ngm_hid_report[] = {
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x05,  // usage (gamepad)
  0xa1, 0x01,  // collection (application)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x01,  // logical maximum (1)
  0x35, 0x00,  // physical minimum (0)
  0x45, 0x01,  // physical minimum (1)
  0x75, 0x01,  // report size (1)
  0x95, 0x0f,  // report count (15)
  0x05, 0x09,  // usage page (button)
  0x19, 0x01,  // usage minimum (1)
  0x29, 0x0f,  // usage maximum (15)
  0x81, 0x02,  // input (variable)
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input (constant)
  0x05, 0x01,  // usage page (desktop)
  0x25, 0x07,  // logical maximum (7)
  0x45, 0x07,  // physical maximum (7)
  0x75, 0x04,  // report size (4)
  0x95, 0x01,  // report count (1)
  0x65, 0x14,  // unit (degrees)
  0x09, 0x39,  // usage (hat switch)
  0x81, 0x42,  // input (variable, null state)
  0x65, 0x00,  // unit
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input (constant)
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x46, 0xff, 0x00,  // physical maximum (255)
  0x09, 0x30,  // usage (x)
  0x09, 0x31,  // usage (y)
  0x09, 0x32,  // usage (z)
  0x09, 0x35,  // usage (rz)
  0x75, 0x08,  // report size (8)
  0x95, 0x04,  // report count (4)
  0x81, 0x02,  // input (variable)
  0x05, 0x02,  // usage page (simulation)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x01,  // logical maximum (1)
  0x45, 0x01,  // physical maximum (1)
  0x09, 0xc5,  // usage (c5h)
  0x09, 0xc4,  // usage (c4h)
  0x09, 0x02,  // usage (02h)
  0x75, 0x01,  // report size (1)
  0x95, 0x03,  // report count (3)
  0x81, 0x02,  // input (variable)
  0x95, 0x05,  // report count (5)
  0x81, 0x01,  // input (constant)
  0xc0,  // end collection
};

static const uint8_t mdm_hid_report[] = {
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x04,  // usage (joystick)
  0xa1, 0x01,  // collection (application)
  0xa1, 0x02,  // collection (logical)
  0x75, 0x08,  // report size (8)
  0x95, 0x05,  // report count (5)
  0x15, 0x00,  // logical minimum (0)
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x35, 0x00,  // physical minimum (0)
  0x46, 0xff, 0x00,  // physical maximum (255)
  0x09, 0x30,  // usage (x)
  0x09, 0x30,  // usage (x)
  0x09, 0x30,  // usage (x)
  0x09, 0x30,  // usage (x)
  0x09, 0x31,  // usage (y)
  0x81, 0x02,  // input
  0x75, 0x04,  // report size (4)
  0x95, 0x01,  // report count (1)
  0x25, 0x07,  // logical maximum (7)
  0x46, 0x3b, 0x01,  // physical maximum (315)
  0x65, 0x14,  // unit (0x14)
  0x09, 0x00,  // usage (undefined)
  0x81, 0x42,  // input
  0x65, 0x00,  // unit (0)
  0x75, 0x01,  // report size (1)
  0x95, 0x0a,  // report count (10)
  0x25, 0x01,  // logical maximum (1)
  0x45, 0x01,  // physical maximum (1)
  0x05, 0x09,  // usage page (button)
  0x19, 0x01,  // usage minimum (1)
  0x29, 0x0a,  // usage maximum (10)
  0x81, 0x02,  // input
  0x06, 0x00, 0xff,  // usage page (vendor)
  0x75, 0x01,  // report size (1)
  0x95, 0x0a,  // report count (10)
  0x25, 0x01,  // logical maximum (1)
  0x45, 0x01,  // physical maximum (1)
  0x09, 0x01,  // usage page (vendor)
  0x81, 0x02,  // input
  0xc0,  // end collection
  0xa1, 0x02,  // collection (logical)
  0x75, 0x08,  // report size (8)
  0x95, 0x04,  // report count (4)
  0x46, 0xff, 0x00,  // physical maximum (255)
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x09, 0x02, // usage (mouse)
  0x91, 0x02, // output
  0xc0,  // end collection
  0xc0,  // end collection
};

static const uint8_t pem_hid_report[] = {
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x05,  // usage (gamepad)
  0xa1, 0x01,  // collection (application)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x01,  // logical maximum (1)
  0x35, 0x00,  // physical minimum (0)
  0x45, 0x01,  // physical maximum (1)
  0x75, 0x01,  // report size (1)
  0x95, 0x0e,  // report count (14)
  0x05, 0x09,  // usage page (button)
  0x19, 0x01,  // usage minimum (1)
  0x29, 0x0e,  // usage maximum (14)
  0x81, 0x02,  // input
  0x95, 0x02,  // report count (2)
  0x81, 0x01,  // input
  0x05, 0x01,  // usage page (desktop)
  0x25, 0x07,  // logical maximum (7)
  0x46, 0x3b, 0x01,  // physical maximum (315)
  0x75, 0x04,  // report size (4)
  0x95, 0x01,  // report count (1)
  0x65, 0x14,  // unit (0x14)
  0x09, 0x39,  // usage (hat switch)
  0x81, 0x42,  // input
  0x65, 0x00,  // unit (0)
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x46, 0xff, 0x00,  // physical maximum (255)
  0x09, 0x30,  // usage (X)
  0x09, 0x31,  // usage (Y)
  0x09, 0x32,  // usage (Z)
  0x09, 0x35,  // usage (Rz)
  0x75, 0x08,  // report size (8)
  0x95, 0x04,  // report count (4)
  0x81, 0x02,  // input
  0x75, 0x08,  // report size (8)
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input
  0x0a, 0x4f, 0x48,  // usage (reserved)
  0x75, 0x08,  // report size (8)
  0x95, 0x08,  // reportr couint (8)
  0xb1, 0x02,  // feature
  0x0a, 0x4f, 0x48,  // usage (reserved)
  0x91, 0x02,  // output
  0xc0,  // end collection
};

static const uint8_t psc_hid_report[] = {
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x05,  // usage (gamepad)
  0xa1, 0x01,  // collection (application)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x01,  // logical maximum (1)
  0x75, 0x01,  // report size (1)
  0x95, 0x0a,  // report count (10)
  0x05, 0x09,  // usage page (button)
  0x19, 0x01,  // usage minimum (1)
  0x29, 0x0a,  // usage maximum (10)
  0x81, 0x02,  // input
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x30,  // usage (X)
  0x09, 0x31,  // usage (Y)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x02,  // logical maximum (2)
  0x35, 0x00,  // physical minimum (0)
  0x45, 0x02,  // physical maximum (2)
  0x75, 0x02,  // report size (2)
  0x95, 0x02,  // report count (2)
  0x81, 0x02,  // input
  0x75, 0x01,  // report size (1)
  0x95, 0x02,  // report count (2)
  0x81, 0x01,  // input
  0xc0,  // end collection
};

const uint8_t* desc_device[] = {
  ngm_device,
  mdm_device,
  pem_device,
  psc_device,
};

const uint8_t* desc_configuration[] = {
  ngm_configuration,
  mdm_configuration,
  pem_configuration,
  psc_configuration,
};

const uint8_t* desc_string_0[] = {
  string_0,
  string_0,
  string_0,
  string_0,
};

const uint8_t* desc_string_1[] = {
  ngm_string_1,
  0,
  pem_string_1,
  psc_string_1,
};

const uint8_t* desc_string_2[] = {
  0,
  mdm_string_2,
  pem_string_2,
  psc_string_2,
};


const uint8_t* desc_hid_report[] = {
  ngm_hid_report,
  mdm_hid_report,
  pem_hid_report,
  psc_hid_report,
};

const uint8_t desc_len_device[] = {
  sizeof(ngm_device),
  sizeof(mdm_device),
  sizeof(pem_device),
  sizeof(psc_device),
};

const uint8_t desc_len_configuration[] = {
  sizeof(ngm_configuration),
  sizeof(mdm_configuration),
  sizeof(pem_configuration),
  sizeof(psc_configuration),
};

const uint8_t desc_len_string_0[] = {
  sizeof(string_0),
  sizeof(string_0),
  sizeof(string_0),
  sizeof(string_0),
};

const uint8_t desc_len_string_1[] = {
  sizeof(ngm_string_1),
  0,
  sizeof(pem_string_1),
  sizeof(psc_string_1),
};

const uint8_t desc_len_string_2[] = {
  0,
  sizeof(mdm_string_2),
  sizeof(pem_string_2),
  sizeof(psc_string_2),
};

const uint8_t desc_len_hid_report[] = {
  sizeof(ngm_hid_report),
  sizeof(mdm_hid_report),
  sizeof(pem_hid_report),
  sizeof(psc_hid_report),
};

static uint8_t get_hat_value(uint16_t buttons, uint16_t* button_masks) {
  uint8_t stick =
      ((buttons & button_masks[B_UP   ]) ? 0x01 : 0) |
      ((buttons & button_masks[B_DOWN ]) ? 0x02 : 0) |
      ((buttons & button_masks[B_LEFT ]) ? 0x04 : 0) |
      ((buttons & button_masks[B_RIGHT]) ? 0x08 : 0);
  switch (stick) {
    case 0x01:  // up
      return 0;
    case 0x02:  // down
      return 4;
    case 0x04:  // left
      return 6;
    case 0x05:  // up-left
      return 7;
    case 0x06:  // down-left
      return 5;
    case 0x08:  // right
      return 2;
    case 0x09:  // up-right
      return 1;
    case 0x0a:  // down-right
      return 3;
    default:
      return 15;
  }
}

uint8_t ngm_in(uint16_t buttons, uint16_t* button_masks, uint8_t* buffer) {
  buffer[0] =
      ((buttons & button_masks[B_3    ]) ? 0x10 : 0) |
      ((buttons & button_masks[B_2    ]) ? 0x08 : 0) |
      ((buttons & button_masks[B_1    ]) ? 0x02 : 0) |
      ((buttons & button_masks[B_4    ]) ? 0x01 : 0);
  buffer[1] =
      ((buttons & button_masks[B_START]) ? 0x08 : 0) |
      ((buttons & button_masks[B_COIN ]) ? 0x04 : 0);
  buffer[2] = get_hat_value(buttons, button_masks);
  for (uint8_t i = 3; i < 8; ++i)
    buffer[i] = 0;
  return 8;
}

uint8_t mdm_in(uint16_t buttons, uint16_t* button_masks, uint8_t* buffer) {
  uint8_t x = 127;
  uint8_t y = 127;
  if (buttons & button_masks[B_LEFT])
    x = 0;
  else if (buttons & button_masks[B_RIGHT])
    x = 255;
  if (buttons & button_masks[B_UP])
    y = 0;
  else if (buttons & button_masks[B_DOWN])
    y = 255;
  buffer[0] = 0;
  buffer[1] = 0;
  buffer[2] = 0;
  buffer[3] = x;
  buffer[4] = y;
  buffer[5] =
      ((buttons & button_masks[B_1]) ? 0x80 : 0) |  // X(3)
      ((buttons & button_masks[B_4]) ? 0x40 : 0) |  // A(2)
      ((buttons & button_masks[B_5]) ? 0x20 : 0) |  // B(1)
      ((buttons & button_masks[B_2]) ? 0x10 : 0);   // Y(0)
  buffer[6] =
      ((buttons & button_masks[B_START]) ? 0x20 : 0) |  // 9
      ((buttons & button_masks[B_COIN]) ? 0x10 : 0) |  // 8
      ((buttons & button_masks[B_6]) ? 0x02 : 0) |  // C(5)
      ((buttons & button_masks[B_3]) ? 0x01 : 0);   // Z(4)
  return 8;
}

uint8_t pem_in(uint16_t buttons, uint16_t* button_masks, uint8_t* buffer) {
  buffer[0] =
      ((buttons & button_masks[B_2    ]) ? 0x04 : 0) |
      ((buttons & button_masks[B_1    ]) ? 0x02 : 0);
  buffer[1] =
      ((buttons & button_masks[B_START]) ? 0x02 : 0) |
      ((buttons & button_masks[B_COIN ]) ? 0x01 : 0);
  buffer[2] = get_hat_value(buttons, button_masks);
  for (uint8_t i = 3; i < 8; ++i)
    buffer[i] = 0;
  return 8;
}

uint8_t get_report(
    uint8_t mode, uint16_t buttons, uint16_t* button_masks, uint8_t* buffer) {
  switch (mode) {
    case NEOGEO_MINI:
      return ngm_in(buttons, button_masks, buffer);
    case MEGADRIVE_MINI:
      return mdm_in(buttons, button_masks, buffer);
    case PC_ENGINE_MINI:
      return pem_in(buttons, button_masks, buffer);
    default:
      return 0;
  }
}