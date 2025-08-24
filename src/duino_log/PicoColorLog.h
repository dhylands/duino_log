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
 *   @file   PicoColorLog.h
 *
 *   @brief  Pico logger which logs to stdout device.
 *
 ****************************************************************************/

#pragma once

#include <stdio.h>

#include "pico/stdlib.h"

#include "duino_log/ConsoleColor.h"
#include "duino_log/Log.h"

//! Class which sends logging output to an Arduino Serial device.
class PicoColorLog : public Log {
 public:
    //! Array of color/prefixes to use for each logging level.
    static const char* level_str[];

    //! Constructor.
    PicoColorLog() : Log() {}

 protected:
    //! Implements the actual logging function.
    void do_log(
        Level level,      //!< Logging level associated with this message.
        const char* fmt,  //!< Printf style format string
        va_list args      //!< Arguments associated with format string.
        ) override {
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

    //! Function called from vStrXPrintf which outputs a single character of output.
    //! @returns 1 if the character was logged successzfully, 0 otherwise.
    static size_t log_char_to_stdout(
        void* outParam,  //!< Pointer to ArduinoSerialLogger object.
        char ch          //!< Character to output.
    ) {
        (void)outParam;
        putc(ch, stdout);
        return 1;
    }
};
