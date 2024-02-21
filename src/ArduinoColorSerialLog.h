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

#pragma once

#include <Arduino.h>

#include "Log.h"

// Attributes
// 0 Reset all attributes
// 1 Bright
// 2 Dim
// 4 Underscore
// 5 Blink
// 7 Reverse
// 8 Hidden
//
// Foreground Colours
// 30    Black
// 31    Red
// 32    Green
// 33    Yellow
// 34    Blue
// 35    Magenta
// 36    Cyan
// 37    White
//
// Background Colours
// 40    Black
// 41    Red
// 42    Green
// 43    Yellow
// 44    Blue
// 45    Magenta
// 46    Cyan
// 47    White

//! Macro to define a console color.
//! @{

#define COLOR_BLACK "\033[1;30m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_WHITE "\033[1;37m"

#define COLOR_DARK_BLACK "\033[2;30m"
#define COLOR_DARK_RED "\033[2;31m"
#define COLOR_DARK_GREEN "\033[2;32m"
#define COLOR_DARK_YELLOW "\033[2;33m"
#define COLOR_DARK_BLUE "\033[2;34m"
#define COLOR_DARK_MAGENTA "\033[2;35m"
#define COLOR_DARK_CYAN "\033[2;36m"
#define COLOR_DARK_WHITE "\033[2;37m"

#define COLOR_BG_BLACK "\033[1;40m"
#define COLOR_BG_RED "\033[1;41m"
#define COLOR_BG_GREEN "\033[1;42m"
#define COLOR_BG_YELLOW "\033[1;43m"
#define COLOR_BG_BLUE "\033[1;44m"
#define COLOR_BG_MAGENTA "\033[1;45m"
#define COLOR_BG_CYAN "\033[1;46m"
#define COLOR_BG_WHITE "\033[1;47m"

#define COLOR_BG_DARK_BLACK "\033[2;40m"
#define COLOR_BG_DARK_RED "\033[2;41m"
#define COLOR_BG_DARK_GREEN "\033[2;42m"
#define COLOR_BG_DARK_YELLOW "\033[2;43m"
#define COLOR_BG_DARK_BLUE "\033[2;44m"
#define COLOR_BG_DARK_MAGENTA "\033[2;45m"
#define COLOR_BG_DARK_CYAN "\033[2;46m"
#define COLOR_BG_DARK_WHITE "\033[2;47m"
#define COLOR_BG_NOCOLOR BG_DARK_BLACK
#define COLOR_NO_COLOR "\033[0m"

//! @}

//! Class which sends logging output to an Arduino Serial device.
class ArduinoColorSerialLog : public Log {
 public:
    //! Array of color/prefixes to use for each logging level.
    static const char* level_str[];

    //! Constructor.
    explicit ArduinoColorSerialLog(
        HardwareSerial* serial  //!< [in] Serial device that output should be sent to
        )
        : serial{serial} {}

 protected:
    //! Implements the actual logging function.
    void do_log(
        Level level,      //!< Logging level associated with this message.
        const char* fmt,  //!< Printf style format string
        va_list args      //!< Arguments associated with format string.
        ) override {
        uint_fast8_t int_level = static_cast<uint_fast8_t>(level);
        if (int_level <= static_cast<uint_fast8_t>(Level::DEBUG)) {
            this->serial->print(level_str[int_level]);
        }
        vStrXPrintf(log_char_to_serial, this, fmt, args);
        this->serial->print(COLOR_NO_COLOR);
        this->serial->print('\r');
        this->serial->print('\n');
    }

    //! Function called from vStrXPrintf which outputs a single character of output.
    //! @returns 1 if the character was logged successzfully, 0 otherwise.
    static size_t log_char_to_serial(
        void* outParam,  //!< Pointer to ArduinoSerialLogger object.
        char ch          //!< Character to output.
    ) {
        auto this_ = reinterpret_cast<ArduinoColorSerialLog*>(outParam);
        this_->serial->print(ch);
        return 1;
    }

 private:
    HardwareSerial* serial;  //!< The serial device that we're logging to.
};
