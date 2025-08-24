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

#include "duino_log/ConsoleColor.h"
#include "duino_log/Log.h"

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
