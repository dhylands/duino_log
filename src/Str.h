/****************************************************************************
 *
 *   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the MIT License version as described in the
 *   LICENSE file in the root of this repository.
 *
 ****************************************************************************/
/**
 *
 *  @file    Str.h
 *
 *  @brief   String Manipulation routines.
 *
 *  Definitions for a variety of string manipulation routine. These routines
 *  are all bounded, to ensure that memory overwrites don't occur.
 *
 ****************************************************************************/

#pragma once

/**
 *  @defgroup   Str   String Manipulation
 *
 *  @brief   Provides a variety of string manipulation routines.
 */

// ---- Include Files -------------------------------------------------------

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @addtogroup Str
 * @{
 */

// ---- Constants and Types -------------------------------------------------

//! Pointer to a function which outputs a single character.
//! @details This function is called by the StrXPrintf()/vStrXPrintf()
//!          functions to output a character. It should return zero
//!          to indicate that the character was not output, and 1 to indicated
//!          that it was output.
using StrXPrintfFunc = size_t (*)(void* outParm, char ch);

// ---- Variable Externs ----------------------------------------------------
// ---- Function Prototypes -------------------------------------------------

//! Bounded variant of strcpy.
//! @details Copies `src` to `dst` but ensures that `dst` (including the
//!          terminating null character) doesn't exceed `maxLen`.
//! @returns a pointer to `dst`
char* StrMaxCpy(
    char* dst,        //!< [out] Place to store destination string.
    const char* src,  //!< [in] Source string to copy.
    size_t maxLen     //!< [in] Maximum length of `dst` (including the terminating null)
);

//! Bounded variant of strcat.
//! @details Concatenates `src` to `dst`, but makes sure that `dst`
//!          (including terminating null), doesn't exceed `maxLen`.
//! @returns a pointer to `dst`.
char* StrMaxCat(
    char* dst,        //!< [in/out] String to concatenate onto.
    const char* src,  //!< [in] String to add to the end of `dst`.
    size_t maxLen     //!< [in] Maximum lengh of `dst` (including the terminating null)
);

//!@}

#if defined(AVR)

#include <avr/pgmspace-fix.h>

size_t StrPrintf_P(char* outStr, size_t maxLen, const char* fmt, ...);
size_t vStrPrintf_P(char* outStr, size_t maxLen, const char* fmt, va_list args);

size_t StrXPrintf_P(StrXPrintfFunc func, void* userParm, const char* fmt, ...);
size_t vStrXPrintf_P(StrXPrintfFunc func, void* userParm, const char* fmt, va_list args);

#define StrPrintf(outStr, maxLen, fmt, args...) StrPrintf_P(outStr, maxLen, PSTR(fmt), ##args)
#define vStrPrintf(outStr, maxLen, fmt, args) vStrPrintf_P(outStr, maxLen, PSTR(fmt), args)

#define StrXPrintf(func, userParm, fmt, args...) StrXPrintf_P(func, userParm, PSTR(fmt), ##args)
#define vStrXPrintf(func, userParm, fmt, args) vStrXPrintf_P(func, userParm, PSTR(fmt), args)

#else

//! @addtogroup StrPrintf
//! @{

size_t StrPrintf(char* outStr, size_t maxLen, const char* fmt, ...)
    __attribute__((format(printf, 3, 4)));
size_t vStrPrintf(char* outStr, size_t maxLen, const char* fmt, va_list args)
    __attribute__((format(printf, 3, 0)));

size_t StrXPrintf(StrXPrintfFunc func, void* userParm, const char* fmt, ...)
    __attribute__((format(printf, 3, 4)));
size_t vStrXPrintf(StrXPrintfFunc func, void* userParm, const char* fmt, va_list args)
    __attribute__((format(printf, 3, 0)));

//! @}

#endif  // defined(AVR)

#if defined(linux)
static inline int stricmp(const char* s1, const char* s2) {
    return strcasecmp(s1, s2);
}
#endif  // defined(linux)

#if defined(__cplusplus)
}
#endif
