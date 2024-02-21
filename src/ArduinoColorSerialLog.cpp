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

#include "ArduinoColorSerialLog.h"

const char* ArduinoColorSerialLog::level_str[] = {
    // clang-format off
                        "",     // NONE
    COLOR_MAGENTA       "[F] ",  // FATAL
    COLOR_RED           "[E] ",  // ERROR
    COLOR_YELLOW        "[W] ",  // WARNING
                        "[I] ",  // INFO
    COLOR_DARK_WHITE    "[D] ",  // DEBUG
    // clang-format off
};
