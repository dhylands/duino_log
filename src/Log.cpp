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
 *   @file   Log.cpp
 *
 *   @brief  Simple logging  infrastructure for Arduino
 *
 ****************************************************************************/

#include "duino_log/Log.h"

#if LOGGING_ENABLED
//! Pointer to the global logger object.
Log* Log::logger = nullptr;
#endif  // LOGGING_ENABLED

void Log::debug(const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(Level::DEBUG)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::DEBUG, fmt, args);
            va_end(args);
        }
    }
}

void Log::info(const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(Level::INFO)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::INFO, fmt, args);
            va_end(args);
        }
    }
}

void Log::warning(const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(Level::WARNING)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::WARNING, fmt, args);
            va_end(args);
        }
    }
}

void Log::error(const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(Level::ERROR)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::ERROR, fmt, args);
            va_end(args);
        }
    }
}

void Log::fatal(const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(Level::FATAL)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::FATAL, fmt, args);
            va_end(args);
        }
    }
}

void Log::log(Level level, const char* fmt, ...) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(level)) {
            va_list args;
            va_start(args, fmt);
            logger->do_log(Level::INFO, fmt, args);
            va_end(args);
        }
    }
}

void Log::vlog(Level level, const char* fmt, va_list args) {
    if constexpr (LOGGING_ENABLED) {
        if (logger != nullptr && logger->should_log(level)) {
            logger->do_log(level, fmt, args);
        }
    }
}
