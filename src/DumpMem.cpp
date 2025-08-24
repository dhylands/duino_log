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
 *   @file   DumpMem.cpp
 *
 *   @brief  Memmory dump routine
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "duino_log/DumpMem.h"

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <climits>

#include "duino_log/Log.h"
#include "duino_log/Str.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

//! Number of bytes that DumpMem will output per line.
static constexpr size_t LINE_WIDTH = 16;

//! Formatted width of a single line out output.
//! @details For each byte output, there are 2 hex digits plus a space, along
//!          with the ASCII equivalent. The +20 allows for the prefix, address
//!          and misc characters.
static constexpr size_t FMT_LINE_WIDTH = LINE_WIDTH * 4 + 20;

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

void DumpLine(
    const char* prefix,
    size_t address,
    const void* inData,
    size_t numBytes,
    size_t lineLen,
    char* line) {
    const uint8_t* data = (const uint8_t*)inData;

    int prefixLen = 0;
    if (*prefix != '\0') {
        prefixLen = StrPrintf(line, lineLen, "%s: ", prefix);
    }

    if (numBytes == 0) {
        StrPrintf(&line[prefixLen], lineLen - prefixLen, "No Data");
        return;
    }

    int len = prefixLen;
    if (address != NO_ADDR) {
        len += StrPrintf(&line[len], lineLen - len, "%04x: ", (unsigned)address);
    }

    // Format the hex portion of the line.
    for (size_t i = 0; i < LINE_WIDTH; i++) {
        if (i < numBytes) {
            len += StrPrintf(&line[len], lineLen - len, "%2.2x ", data[i]);
        } else {
            len += StrPrintf(&line[len], lineLen - len, "   ");
        }
    }

    if (len + numBytes + 1 < lineLen) {
        // Format the ASCII portion of the line.
        for (size_t i = 0; i < LINE_WIDTH; i++) {
            if (i < numBytes) {
                if (std::isprint(data[i])) {
                    line[len++] = data[i];
                } else {
                    line[len++] = '.';
                }
            } else {
                break;
            }
        }

        line[len++] = '\0';
    }
}  // DumpLine

void DumpMem(const char* prefix, size_t address, const void* inData, size_t numBytes) {
    auto data = reinterpret_cast<const uint8_t*>(inData);

    char line[FMT_LINE_WIDTH];

    if (numBytes == 0) {
        DumpLine(prefix, address, data, numBytes, sizeof(line), line);
        Log::info("%s", line);
        return;
    }

    for (size_t i = 0; i < numBytes; i += LINE_WIDTH) {
        size_t bytesThisLine = std::min(numBytes - i, LINE_WIDTH);
        DumpLine(prefix, address, data, bytesThisLine, sizeof(line), line);
        Log::info("%s", line);

        address += bytesThisLine;
        data += bytesThisLine;
    }
}  // DumpMem

std::ostream& operator<<(std::ostream& out, const dump& d) {
    const uint8_t* data = (const uint8_t*)d.data;

    char line[FMT_LINE_WIDTH];

    out << std::endl;

    if (d.numBytes == 0) {
        DumpLine(d.prefix, d.address, d.data, d.numBytes, sizeof(line), line);
        out << line << std::endl;
        return out;
    }

    unsigned address = d.address;
    for (size_t i = 0; i < d.numBytes; i += LINE_WIDTH) {
        size_t bytesThisLine = std::min(d.numBytes - i, LINE_WIDTH);
        DumpLine(d.prefix, address, data, bytesThisLine, sizeof(line), line);
        out << line << std::endl;

        address += bytesThisLine;
        data += bytesThisLine;
    }
    return out;
}
