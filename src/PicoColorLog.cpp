/****************************************************************************
 *
 *   @copyright Copyright (c) 2024 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the MIT License version as described in the
 *   LICENSE file in the root of this repository.
 *
 ****************************************************************************/
/**
 *   @file   ArduinoColorSerialLog.h
 *
 *   @brief  Arduino logger which logs to the Serial device.
 *
 ****************************************************************************/

#include "duino_log/PicoColorLog.h"

#include <stdarg.h>
#include <stdio.h>

#include "pico/stdlib.h"

#include "duino_log/ConsoleColor.h"

const char* PicoColorLog::level_str[] = {
    // clang-format off
                        "",     // NONE
    COLOR_MAGENTA       "[F] ",  // FATAL
    COLOR_RED           "[E] ",  // ERROR
    COLOR_YELLOW        "[W] ",  // WARNING
                        "[I] ",  // INFO
    COLOR_DARK_WHITE    "[D] ",  // DEBUG
    // clang-format off
};

size_t PicoColorLog::log_char_to_stdout(void* outParam, char ch) {
    (void)outParam;
    putc(ch, stdout);
    return 1;
}

void PicoColorLog::do_log(Level level, const char* fmt, va_list args) {
    uint_fast8_t int_level = static_cast<uint_fast8_t>(level);
    if (int_level <= static_cast<uint_fast8_t>(Level::DEBUG)) {
        fputs(level_str[int_level], stdout);
    }
    vStrXPrintf(log_char_to_stdout, this, fmt, args);
    fputs(COLOR_NO_COLOR, stdout);
    putc('\r', stdout);
    putc('\n', stdout);
    fflush(stdout);
}
