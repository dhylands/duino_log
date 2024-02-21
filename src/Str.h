/****************************************************************************
 *
 *   @copyright Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the MIT License version as described in the
 *   LICENSE file in the root of this repository.
 *
 ****************************************************************************/
/**
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
using StrXPrintfFunc = size_t (*)(void* outParm, char ch) noexcept;

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
    ) noexcept;

//! Bounded variant of strcat.
//! @details Concatenates `src` to `dst`, but makes sure that `dst`
//!          (including terminating null), doesn't exceed `maxLen`.
//! @returns a pointer to `dst`.
char* StrMaxCat(
    char* dst,        //!< [in/out] String to concatenate onto.
    const char* src,  //!< [in] String to add to the end of `dst`.
    size_t maxLen     //!< [in] Maximum lengh of `dst` (including the terminating null)
    ) noexcept;

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

//! Writes formatted data into a user supplied buffer.
//! @return  The number of characters actually contained in `outStr`, not
//!          including the temrinating null character. This differs slightly
//!          from snprintf which returns the number of characters that would
//!          have been written if the output buffer were unlimited in size.
size_t StrPrintf(
    char* outStr,     //!< [out] Place to store formatted string.
    size_t maxLen,    //!< [in] Length out `outStr`.
    const char* fmt,  //!< [in] Printf style format string.
    ...               //!< [in] Varadic arguments associated with format string.
    ) noexcept __attribute__((format(printf, 3, 4)));

//! Writes formatted data into a user supplied buffer.
//! @return  The number of characters actually contained in `outStr`, not
//!          including the temrinating null character. This differs slightly
//!          from snprintf which returns the number of characters that would
//!          have been written if the output buffer were unlimited in size.
size_t vStrPrintf(
    char* outStr,     //!< [out] Place to store formatted string.
    size_t maxLen,    //!< [in] Length out `outStr`.
    const char* fmt,  //!< [in] Printf style format string.
    va_list args      //!< [in] Arguments associated with the format string.
    ) noexcept __attribute__((format(printf, 3, 0)));

//! Generic printf function which writes formatted data by calling a user supplied function.
//! @details func() will be called to output each character. If func return 1, then
//!          StrXPrintf() will continue to call func() with addition characters.
//! @return  The number of characters actually contained in `outStr`, not
//!          including the temrinating null character. This differs slightly
//!          from snprintf which returns the number of characters that would
//!          have been written if the output buffer were unlimited in size.
size_t StrXPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    ...                   //!< [in] Varadic arguments associated with format string.
    ) noexcept __attribute__((format(printf, 3, 4)));

//! Generic, reentrant printf function.
//! @details This is the workhorse of the StrPrintf functions.
//!
//!          func() will be called to output each character. If func return 1, then
//!          StrXPrintf() will continue to call func() with addition characters.
//!
//!  The format string `fmt` consists of ordinary characters, escape
//!  sequences, and format specifications. The ordinary characters and escape
//!  sequences are output in their order of appearance. Format specifications
//!  start with a percent sign (%) and are read from left to right. When
//!  the first format specification (if any) is encountered, it converts the
//!  value of the first argument after `fmt` and outputs it accordingly.
//!  The second format specification causes the second argument to be
//!  converted and output, and so on. If there are more arguments than there
//!  are format specifications, the extra arguments are ignored. The
//!  results are undefined if there are not enough arguments for all the
//!  format specifications.
//!
//!  A format specification has optional, and required fields, in the following
//!  form:
//!
//!     %[flags][width][.precision][l]type
//!
//!  Each field of the format specification is a single character or a number
//!  specifying a particular format option. The simplest format specification
//!  contains only the percent sign and a @b type character (for example %s).
//!  If a percent sign is followed by a character that has no meaning as a
//!  format field, the character is sent to the output function. For example,
//!  to print a percent-sign character, use %%.
//!
//!  The optional fields, which appear before the type character, control
//!  other aspects of the formatting, as follows:
//!
//!  @b flags may be one of the following:
//!
//!  - - (minus sign) left align the result within the given field width.
//!  - 0 (zero) Zeros are added until the minimum width is reached.
//!
//!  @b width may be one of the following:
//!  - a number specifying the minimum width of the field
//!  - * (asterick) means that an integer taken from the argument list will
//!    be used to provide the width. The @a width argument must precede the
//!    value being formatted in the argument list.
//!
//!  @b precision may be one of the following:
//!  - a number
//!  - * (asterick) means that an integer taken from the argument list will
//!    be used to provide the precision. The @a precision argument must
//!    precede the value being formatted in the argument list.
//!
//!  The interpretation of @a precision depends on the type of field being
//!  formatted:
//!  - For b, d, o, u, x, X, the precision specifies the minimum number of
//!    digits that will be printed. If the number of digits in the argument
//!    is less than @a precision, the output value is padded on the left with
//!    zeros. The value is not truncated when the number of digits exceeds
//!    @a prcision.
//!  - For s, the precision specifies the maximum number of characters to be
//!    printed.
//!
//!  The optional type modifier l (lowercase ell), may be used to specify
//!  that the argument is a long argument. This makes a difference on
//!  architectures where the sizeof an int is different from the sizeof a long.
//!
//!  @b type causes the output to be formatted as follows:
//!  - b Unsigned binary integer.
//!  - c Character.
//!  - d Signed decimal integer.
//!  - o Unsigned octal integer.
//!  - s Null terminated character string.
//!  - u Unsigned Decimal integer.
//!  - x Unsigned hexadecimal integer, using "abcdef".
//!  - X Unsigned hexadecimal integer, using "ABCDEF".
//!
//! @return  The number of characters actually contained in `outStr`, not
//!          including the temrinating null character. This differs slightly
//!          from snprintf which returns the number of characters that would
//!          have been written if the output buffer were unlimited in size.
size_t vStrXPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    va_list args          //!< [in] Arguments associated with the format string.
    ) noexcept __attribute__((format(printf, 3, 0)));

//! Variants of the StrPrintf function which doesn't do attribute checking.
//! @details StrPrintf supports %b, but that isn't an official format specifier, and
//!          the compiler will generate a warning/error if you use it. These variants
//!          which are defined as aliases in the .cpp file, allow using the %b format
//!          specifier with the caveat that no other format specifier checking is done.
//! @return  The number of characters actually contained in `outStr`, not
//!          including the temrinating null character. This differs slightly
//!          from snprintf which returns the number of characters that would
//!          have been written if the output buffer were unlimited in size.
//! @{
size_t StrBPrintf(
    char* outStr,     //!< [out] Place to store formatted string.
    size_t maxLen,    //!< [in] Length out `outStr`.
    const char* fmt,  //!< [in] Printf style format string.
    ...               //!< [in] Varadic arguments associated with format string.
    ) noexcept;
size_t vStrBPrintf(
    char* outStr,     //!< [out] Place to store formatted string.
    size_t maxLen,    //!< [in] Length out `outStr`.
    const char* fmt,  //!< [in] Printf style format string.
    va_list args      //!< [in] Arguments associated with the format string.
    ) noexcept;
size_t StrXBPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    ...                   //!< [in] Varadic arguments associated with format string.
    ) noexcept;
size_t vStrXBPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    va_list args          //!< [in] Arguments associated with the format string.
    ) noexcept;
//! @}

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
