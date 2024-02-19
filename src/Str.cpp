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
 *  Str.c
 *
 *  PURPOSE:
 *
 *   This implements a sample program for accessing the serial port.
 *
 ****************************************************************************/

// ---- Include Files --------------------------------------------------------

#include <string.h>

#include <cassert>

#include "Str.h"

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

char* StrMaxCat(char* dst, const char* src, size_t maxLen) {
    size_t dstLen = strlen(dst);

    if (dstLen < maxLen) {
        StrMaxCpy(&dst[dstLen], src, maxLen - dstLen);
    }

    return dst;
}

char* StrMaxCpy(char* dst, const char* src, size_t maxLen) {
    assert(maxLen > 0);

    if (maxLen == 1) {
        /*
         * There's only room for the terminating null character
         */

        dst[0] = '\0';
        return dst;
    }

    /*
     * The Visual C++ version of strncpy writes to every single character
     * of the destination buffer, so we use a length one character smaller
     * and write in our own null (if required).
     *
     * This allows the caller to store a sentinel in the last byte of the
     * buffer to detect overflows (if desired).
     */

    strncpy(dst, src, maxLen - 1);
    if ((strnlen(src, maxLen) + 1) >= maxLen) {
        /*
         * The string exactly fits, or probably overflows the buffer.
         * Write in the terminating null character since strncpy doesn't in
         * this particular case.
         *
         * We don't do this arbitrarily so that the caller can use a sentinel
         * in the very end of the buffer to detect buffer overflows.
         */

        dst[maxLen - 1] = '\0';
    }

    return dst;
}
