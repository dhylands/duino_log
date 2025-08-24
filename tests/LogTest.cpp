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
 *   @file   LogTest.cpp
 *
 *   @brief  Tests for functions in Log.h
 *
 ****************************************************************************/

#include <stdarg.h>
#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "duino_log/Log.h"
#include "duino_util/Util.h"

//! Logger used for tesing Log messages.
class TestLog : public Log {
 public:
    Level last_level;  //!< Level of last log message that was used.
    std::string line;  //!< Accumulated output.

 protected:
    //! Function which records the logged message.
    void do_log(
        Level level,      //!< [in] Level associated with this message.
        const char* fmt,  //!< [in] printf style format string.
        va_list args      //!< [in] List of parameters
        ) noexcept override {
        char log_line[100];
        this->last_level = level;
        vStrPrintf(log_line, LEN(log_line), fmt, args);
        if (this->line.length() > 0) {
            this->line.push_back('\n');
        }
        this->line.append(log_line);
    }
};

TEST(LogDeathTest, DoubleLogInstance) {
    TestLog log1;

    ASSERT_DEATH({ TestLog log2; }, "Assertion `logger == nullptr' failed.");
}

//! Helper function for testing Log::vlog
static void test_vlog(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Log::vlog(Log::Level::INFO, fmt, args);
    va_end(args);
}

//! Test normal logs
TEST(LogTest, LogMessagges) {
    TestLog log;

    EXPECT_EQ(log.get_level(), Log::Level::DEBUG);

    Log::debug("This is a debug log");
    EXPECT_EQ(log.last_level, Log::Level::DEBUG);
    EXPECT_STREQ(log.line.c_str(), "This is a debug log");

    Log::debug("Second line");
    EXPECT_EQ(log.last_level, Log::Level::DEBUG);
    EXPECT_STREQ(log.line.c_str(), "This is a debug log\nSecond line");

    log.line.clear();
    Log::info("This is an info log");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is an info log");

    Log::info("Second line");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is an info log\nSecond line");

    log.line.clear();
    Log::warning("This is a warning log");
    EXPECT_EQ(log.last_level, Log::Level::WARNING);
    EXPECT_STREQ(log.line.c_str(), "This is a warning log");

    Log::warning("Second line");
    EXPECT_EQ(log.last_level, Log::Level::WARNING);
    EXPECT_STREQ(log.line.c_str(), "This is a warning log\nSecond line");

    log.line.clear();
    Log::error("This is an error log");
    EXPECT_EQ(log.last_level, Log::Level::ERROR);
    EXPECT_STREQ(log.line.c_str(), "This is an error log");

    Log::error("Second line");
    EXPECT_EQ(log.last_level, Log::Level::ERROR);
    EXPECT_STREQ(log.line.c_str(), "This is an error log\nSecond line");

    log.line.clear();
    Log::fatal("This is a fatal log");
    EXPECT_EQ(log.last_level, Log::Level::FATAL);
    EXPECT_STREQ(log.line.c_str(), "This is a fatal log");

    Log::fatal("Second line");
    EXPECT_EQ(log.last_level, Log::Level::FATAL);
    EXPECT_STREQ(log.line.c_str(), "This is a fatal log\nSecond line");

    log.line.clear();
    Log::log(Log::Level::INFO, "This is an info log");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is an info log");

    Log::log(Log::Level::INFO, "Second line");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is an info log\nSecond line");

    log.line.clear();
    test_vlog("This is a vlog log");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is a vlog log");

    test_vlog("Second line");
    EXPECT_EQ(log.last_level, Log::Level::INFO);
    EXPECT_STREQ(log.line.c_str(), "This is a vlog log\nSecond line");
}

//! Test disabled logs
TEST(LogTest, LogMessaggesTurnedOffByLogLevel) {
    TestLog log;

    log.set_level(Log::Level::NONE);
    EXPECT_EQ(log.get_level(), Log::Level::NONE);

    log.line.clear();
    Log::debug("This is a debug log");
    EXPECT_STREQ(log.line.c_str(), "");

    Log::info("This is an info log");
    EXPECT_STREQ(log.line.c_str(), "");

    Log::warning("This is a warning log");
    EXPECT_STREQ(log.line.c_str(), "");

    Log::error("This is an error log");
    EXPECT_STREQ(log.line.c_str(), "");

    Log::fatal("This is a fatal log");
    EXPECT_STREQ(log.line.c_str(), "");

    Log::log(Log::Level::INFO, "This is an info log");
    EXPECT_STREQ(log.line.c_str(), "");

    test_vlog("This is a vlog log");
    EXPECT_STREQ(log.line.c_str(), "");
}

//! These test the logger != nullptr portion of the if test
TEST(LogTest, NoLogger) {
    Log::debug("This is a debug log");
    Log::info("This is an info log");
    Log::warning("This is a warning log");
    Log::error("This is an error log");
    Log::fatal("This is a fatal log");
    Log::log(Log::Level::INFO, "This is an info log");
    test_vlog("This is a vlog log");
}
