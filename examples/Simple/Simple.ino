/****************************************************************************
 *
 *   Copyright (c) 2024 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   Alternatively, this software may be distributed under the terms of BSD
 *   license.
 *
 *   See README and COPYING for more details.
 *
 ****************************************************************************/
/**
 *   @file   Simple.ino
 *
 *   @brief  Simple program for demonstrating the ArduinoSimpleLog.
 *
 ****************************************************************************/

#include "DuinoLog.h"

#include <cinttypes>

//! Global logger object
ArduinoSerialLog serial_log{&Serial};

//! Perform one-time setup
void setup(void) {
    Serial.begin();
}

static uint32_t counter = 0;

//! Runs indefinitely
void loop(void) {
    Log::info("Test: %" PRIu32, counter);
    counter++;
    delay(1000);
}
