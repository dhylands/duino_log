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
 *   @file   ConsoleColor.h
 *
 *   @brief  Defines colors for printing on a linux console.
 *
 ****************************************************************************/

#pragma once

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

//! @brief Macro to define a console color.
//! @details These are done as macros rather than const char*'s so that
//!          token pasting
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
