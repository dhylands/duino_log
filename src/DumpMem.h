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
 *   @file   DumpMem.h
 *
 *   @brief  Debug routine for dumping memory
 *
 ****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <ostream>
#include <cstddef>

/**
 * @addtogroup Log
 * @{
 */

//! Constant to suppress printing of the address.
static constexpr size_t NO_ADDR = SIZE_MAX;

//! Formats a line of dump data into a buffer.
void DumpLine(
    char const* prefix,  //!< [in] String to prefix each line of output with.
    size_t address,      //!< [in] Address to print for the first byteof the data.
    const void* data,    //!< [in] Pointer to the data.
    size_t numBytes,     //!< [in] number of bytes of data.
    size_t lineLen,      //!< [in] Length of output buffer.
    char* line           //!< [out] Place to store formatted line.
);

//! Dumps a page of output for debugging purposes.
void DumpMem(
    char const* prefix,  //!< [in] String to prefix each line of output with.
    size_t address,      //!< [in] Address to print for the first byteof the data.
    const void* data,    //!< [in] Pointer to the data.
    size_t numBytes      //!< [in] number of bytes of data.
);

//! Streaming object which allows outut to be sent to a stream.
class dump {
 public:
    //! @brief  Constructor which assumes prefix="" and address=0
    dump(
        const void* data,  //!< [in] Data to dump
        size_t numBytes    //!< [in] Number of bytes to dump
        )
        : prefix(""), address(0), data(data), numBytes(numBytes) {}

    //! Constructor which dumps a regions of memory and produces formatted output.
    dump(
        char const* prefix,  //!< [in] String to prefix each line of output with.
        size_t address,      //!< [in] Address to print for the first byteof the data.
        const void* data,    //!< [in] Pointer to the data.
        size_t numBytes      //!< [in] number of bytes of data.
        )
        : prefix(prefix), address(address), data(data), numBytes(numBytes) {}

    //! Streaming operator
    //! @returns the stream being operated on.
    std::ostream& operator<<(std::ostream&  //!< [in] Stream that the output should go to.
    ) const;

 private:
    friend std::ostream& operator<<(std::ostream& out, const dump& d);

    const char* prefix;  //!< Prefix that each line should be prefixed with.
    size_t address;      //!< Address to print that corresponds to byte 0.
    const void* data;    //!< Data to dump.
    size_t numBytes;     //!< Number of bytes to dump.
};

//! C++ manipulator for dumping a buffer's worth of data.
//! @returns the stream being dumped to.
std::ostream& operator<<(
    std::ostream& out,  //!< [in] Stream that output should go to.
    const dump& d       //!< [in] dump object describing the data to format.
);

/** @} */
