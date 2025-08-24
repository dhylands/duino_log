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
 *   @file   DumpMemTest.cpp
 *
 *   @brief  Tests for functions in DumpMem.cpp
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>
#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "duino_log/DumpMem.h"
#include "duino_log/Log.h"
#include "duino_log/Str.h"
#include "duino_util/Util.h"

static uint8_t data[] = {
    0x00, 0x01, 0x02, 0x31, 0x32, 0x33, 0x41, 0x42, 0x43, 0x11, 0x12, 0x13, 0x36, 0x37, 0x38, 0x39,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
};

//! Logger used for testing DumpMem
class DumpMemLogger : public Log {
 public:
    std::string str;  //!< String that output is accumulated into.

 protected:
    //! Function that actually performs the logging.
    void do_log(
        Level level,      //!< [in] Logging level associated with the current messgae.
        const char* fmt,  //!< [in] Printf style format string.
        va_list args      //!< [in] Arguments associated with the format string.
        ) noexcept override {
        (void)level;
        char line[100];

        vStrPrintf(line, LEN(line), fmt, args);

        if (this->str.length() > 0) {
            this->str.push_back('\n');
        }
        this->str.append(line);
    }
};

//! Test Fixture for testing DumpMem
class DumpMemTestFixture : public ::testing::Test {
 protected:
    DumpMemLogger logger;  //!< The logger used to capture output of DumpMem.
};

TEST(DumpLineTest, DumpLineNoData) {
    char line[100];

    DumpLine("Test", 0, data, 0, LEN(line), line);

    EXPECT_STREQ(line, "Test: No Data");
}

TEST(DumpLineTest, DumpLineOneByte) {
    char line[100];

    DumpLine("Test", 0, data, 1, LEN(line), line);

    EXPECT_STREQ(line, "Test: 0000: 00                                              .");
}

TEST(DumpLineTest, DumpLineOneByteNoAddr) {
    char line[100];

    DumpLine("Test", NO_ADDR, data, 1, LEN(line), line);

    EXPECT_STREQ(line, "Test: 00                                              .");
}

TEST(DumpLineTest, DumpLineOneLine) {
    char line[100];

    DumpLine("Test", 0, data, 16, LEN(line), line);

    EXPECT_STREQ(
        line, "Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789");
}

TEST(DumpLineTest, DumpLineOneLineTruncated) {
    char line[65];

    DumpLine("Test", 0, data, 16, LEN(line), line);

    EXPECT_STREQ(line, "Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ");
}

TEST_F(DumpMemTestFixture, DumpMemNoData) {
    DumpMem("Test", 0, data, 0);
    EXPECT_STREQ(logger.str.c_str(), "Test: No Data");
}

TEST_F(DumpMemTestFixture, DumpMemOneByte) {
    std::string s;

    DumpMem("Test", 0, data, 1);

    EXPECT_STREQ(
        logger.str.c_str(), "Test: 0000: 00                                              .");
}

TEST_F(DumpMemTestFixture, DumpMemOneByteNoAddr) {
    DumpMem("Test", NO_ADDR, data, 1);

    EXPECT_STREQ(logger.str.c_str(), "Test: 00                                              .");
}

TEST_F(DumpMemTestFixture, DumpMemOneLine) {
    DumpMem("Test", 0, data, 16);

    EXPECT_STREQ(
        logger.str.c_str(),
        "Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789");
}

TEST_F(DumpMemTestFixture, DumpMemOnePlusLines) {
    DumpMem("Test", 0, data, 24);

    EXPECT_STREQ(
        logger.str.c_str(),
        "Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789\n"
        "Test: 0010: 20 21 22 23 24 25 61 62                          !\"#$%ab");
}

TEST_F(DumpMemTestFixture, DumpMemTwoLines) {
    DumpMem("Test", 0, data, 32);

    EXPECT_STREQ(
        logger.str.c_str(),
        "Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789\n"
        "Test: 0010: 20 21 22 23 24 25 61 62 63 64 65 66 67 68 69 6a  !\"#$%abcdefghij");
}

TEST(DumpMemStreamTest, NoDataSimpleConstructor) {
    std::ostringstream output;

    output << dump(data, 0);

    EXPECT_STREQ(output.str().c_str(), "\nNo Data\n");
}

TEST(DumpMemStreamTest, NoDataFullConstructor) {
    std::ostringstream output;

    output << dump("Test", 0, data, 0);

    EXPECT_STREQ(output.str().c_str(), "\nTest: No Data\n");
}

TEST(DumpMemStreamTest, DataSimpleConstructor) {
    std::ostringstream output;

    output << dump(data, 24);

    EXPECT_STREQ(
        output.str().c_str(),
        "\n0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789\n"
        "0010: 20 21 22 23 24 25 61 62                          !\"#$%ab\n");
}

TEST(DumpMemStreamTest, DataFullConstructor) {
    std::ostringstream output;

    output << dump("Test", 0, data, 24);

    EXPECT_STREQ(
        output.str().c_str(),
        "\nTest: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789\n"
        "Test: 0010: 20 21 22 23 24 25 61 62                          !\"#$%ab\n");
}
