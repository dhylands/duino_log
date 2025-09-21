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

#include "duino_log/Log.h"

//! Class which sends logging output to an Arduino Serial device.
class PicoColorLog : public Log {
 public:
    //! Array of color/prefixes to use for each logging level.
    static const char* level_str[];

    //! Constructor.
    PicoColorLog() : Log() {}

    //! Desructor.
    ~PicoColorLog() override = default;

 protected:
    //! Implements the actual logging function.
    void do_log(
        Level level,      //!< Logging level associated with this message.
        const char* fmt,  //!< Printf style format string
        va_list args      //!< Arguments associated with format string.
        ) override;

    //! Function called from vStrXPrintf which outputs a single character of output.
    //! @returns 1 if the character was logged successzfully, 0 otherwise.
    static size_t log_char_to_stdout(
        void* outParam,  //!< Pointer to ArduinoSerialLogger object.
        char ch          //!< Character to output.
    );
};
