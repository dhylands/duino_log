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
 *   @file   StrTest.cpp
 *
 *   @brief  Tests for functions in Str.cpp
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>
#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "Str.h"
#include "Util.h"

TEST(StrMaxCpyTest, Normal) {
    char dst[20];
    char* result;

    result = StrMaxCpy(dst, "This ", 1);
    EXPECT_EQ(result, dst);
    EXPECT_STREQ(result, "");

    result = StrMaxCpy(dst, "This ", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_STREQ(result, "This ");

    result = StrMaxCpy(dst, "This ", 0);
    // We passed in a length of zero, which means that StrMaxCpy can't do
    // anything except return the destination. So we make sure that dst
    // has the same value as it did before we called StrMaxCpy.
    EXPECT_STREQ(result, "This ");
    EXPECT_EQ(result, dst);

    result = StrMaxCpy(dst, "This is a test to see", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_STREQ(result, "This is a test to s");
}

TEST(StrMaxCatTest, Normal) {
    char dst[30];
    char* result;

    result = StrMaxCpy(dst, "This ", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 5);

    result = StrMaxCat(dst, "is a test", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 14);

    result = StrMaxCpy(dst, "This is a test to see what", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 26);

    // We're concatenating to a string that's already too long. StrMaxCat will truncate
    // the destination.
    result = StrMaxCat(dst, "happens", 10);
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 9);

    // Since we're passing in a length of zero, StrMaxCat can't modify anything
    // so it just returns dst with the previous contents.
    result = StrMaxCat(dst, "happens", 0);
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 9);
}

TEST(StrMaxCatTest, Truncated) {
    char dst[12];
    char* result;

    result = StrMaxCpy(dst, "This ", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_STREQ(result, "This ");
    result = StrMaxCat(dst, "is a test", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 11);

    result = StrMaxCat(dst, " to see", LEN(dst));
    EXPECT_EQ(result, dst);
    EXPECT_EQ(strlen(dst), 11);
}
