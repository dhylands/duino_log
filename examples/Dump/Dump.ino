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
 *   @file   Dump.ino
 *
 *   @brief  Simple program for demonstrating the ArduinoSimpleLog.
 *
 ****************************************************************************/

#include <cinttypes>

#include "DuinoLog.h"

//! Global logger object
ArduinoSerialLog serial_log{&Serial};

uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 'T',  'h',  'i',  's',  ' ',
                  'i',  's',  ' ',  'a',  ' ',  't',  'e',  's',  't',  0};

//! Perform one-time setup
void setup(void) {
    Serial.begin();
}

static uint32_t counter = 0;

char line[100];

//! Runs indefinitely
void loop(void) {
    Log::info("Test: %" PRIu32, counter);
    counter++;

    DumpMem("   Data", 0, data, LEN(data));
    DumpLine("   Line", 0, data, 16, LEN(line), line);
    Serial.printf("%s\r\n", line);
    DumpLine("   Line", 16, &data[16], LEN(data) - 16, LEN(line), line);
    Serial.printf("%s\r\n", line);
    DumpMem("Counter", 0, &counter, sizeof(counter));
    delay(1000);
}
