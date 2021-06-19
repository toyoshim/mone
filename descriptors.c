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
  0x08,  // max packet size
  0xbc, 0x20,  // vendor ID
  0x00, 0x55,  // device ID
  0x00, 0x01,  // device version
  0x01,  // manufacturer string index
  0x00,  // product string index
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

  0x09,  // size
  USB_DESC_HID,
  0x01, 0x01,  // BCD representation of HID verrsion
  0x00,  // target country code
  0x01,  // number of HID report
  USB_DESC_HID_REPORT,
  0x61, 0x00,

  // endpoint descriptor
  0x07,  // size
  USB_DESC_ENDPOINT,
  0x81,  // IN endpoint number 1
  0x03,  // attribute: interrurpt endpoint
  0x80, 0x00,  // maximum packet size
  0x0a,  // poll interval 10ms
};

static const uint8_t ngm_string_0[] = { 0x04, 0x03, 0x09, 0x04 };  // language descriptor
static const uint8_t ngm_string_1[] = { 0x06, 0x03, 'J', 0, 'J', 0 };

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

const uint8_t* desc_device[] = {
  ngm_device,
};

const uint8_t* desc_configuration[] = {
  ngm_configuration,
};

const uint8_t* desc_string_0[] = {
  ngm_string_0,
};

const uint8_t* desc_string_1[] = {
  ngm_string_1,
};

const uint8_t* desc_hid_report[] = {
  ngm_hid_report,
};

const uint8_t desc_len_device[] = {
  sizeof(ngm_device),
};

const uint8_t desc_len_configuration[] = {
  sizeof(ngm_configuration),
};

const uint8_t desc_len_string_0[] = {
  sizeof(ngm_string_0),
};

const uint8_t desc_len_string_1[] = {
  sizeof(ngm_string_1),
};

const uint8_t desc_len_hid_report[] = {
  sizeof(ngm_hid_report),
};