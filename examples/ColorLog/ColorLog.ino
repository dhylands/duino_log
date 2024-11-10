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
 *   @file   ColorLog.ino
 *
 *   @brief  Sample Arduino Log which generates colored output.
 *
 ****************************************************************************/

#include <cinttypes>

#include "duino_log.h"

//! Global logger object
ArduinoColorSerialLog serial_log{&Serial};

//! Perform one-time setup
void setup(void) {
    Serial.begin(115200);
}

static uint32_t counter = 0;

//! Runs indefinitely
void loop(void) {
    Log::debug("Test: %" PRIu32, counter);
    Log::info("Test: %" PRIu32, counter);
    Log::warning("Test: %" PRIu32, counter);
    Log::error("Test: %" PRIu32, counter);
    Log::fatal("Test: %" PRIu32, counter);
    Log::debug("This is a debug message");
    Log::info("This is an info message");
    Log::warning("This is a warning message");
    Log::error("This is a error message");
    Log::fatal("This is a fatal message");
    counter++;
    delay(1000);
}
