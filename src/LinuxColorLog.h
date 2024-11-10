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
 *   @file   LinuxColorLog.h
 *
 *   @brief  Logger which prints colors to the console under linux.
 *
 ****************************************************************************/

#pragma once

#include <cstdio>

#include "ConsoleColor.h"
#include "Log.h"

//! Class which sends logging output to an Arduino Serial device.
class LinuxColorLog : public Log {
 public:
    //! Array of color/prefixes to use for each logging level.
    static const char* level_str[];

    //! @brief Constructor
    LinuxColorLog(FILE* log_fs  //!< [in] File to send logging output to.
                  )
        : m_log_fs{log_fs} {}

 protected:
    //! Implements the actual logging function.
    void do_log(
        Level level,      //!< Logging level associated with this message.
        const char* fmt,  //!< Printf style format string
        va_list args      //!< Arguments associated with format string.
        ) override;

 private:
    static size_t log_char_to_file(
        void* outParam,  //!< Pointer to ArduinoSerialLogger object.
        char ch          //!< Character to output.
    );

    FILE* m_log_fs;  //!< File Stream to log to.
};
