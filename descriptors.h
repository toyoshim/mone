// Copyright 2021 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef __descriptors_h__
#define __descriptors_h__

#include <stdint.h>

uint8_t descriptors_size(uint8_t type, uint8_t no);
const uint8_t* descriptors_get(uint8_t type, uint8_t no);
uint8_t descriptors_report(uint8_t* buffer);

#endif