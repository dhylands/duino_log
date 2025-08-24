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
 *   @file   LinuxColorLog.cpp
 *
 *   @brief  Logger which prints colors to the console under linux.
 *
 ****************************************************************************/

#include "duino_log/LinuxColorLog.h"

#include <cstdio>

#include "duino_log/Str.h"

const char* LinuxColorLog::level_str[] = {
    // clang-format off
                        "",     // NONE
    COLOR_MAGENTA       "[F] ",  // FATAL
    COLOR_RED           "[E] ",  // ERROR
    COLOR_YELLOW        "[W] ",  // WARNING
                        "[I] ",  // INFO
    COLOR_DARK_WHITE    "[D] ",  // DEBUG
    // clang-format off
};

//! Function called from vStrXPrintf which outputs a single character of output.
//! @returns 1 if the character was logged successzfully, 0 otherwise.
size_t LinuxColorLog::log_char_to_file(
    void* outParam,  //!< Pointer to ArduinoSerialLogger object.
    char ch          //!< Character to output.
) {
    auto this_ = reinterpret_cast<LinuxColorLog*>(outParam);
    fputc(ch, this_->m_log_fs);
    return 1;
}

void LinuxColorLog::do_log(Level level, const char* fmt, va_list args) {
    uint_fast8_t int_level = static_cast<uint_fast8_t>(level);
    if (int_level <= static_cast<uint_fast8_t>(Level::DEBUG)) {
        fputs(level_str[int_level], this->m_log_fs);
    }
    vStrXPrintf(log_char_to_file, this, fmt, args);
    fputs(COLOR_NO_COLOR, this->m_log_fs);
    fputc('\n', this->m_log_fs);
    fflush(this->m_log_fs);
}
