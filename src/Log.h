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
 *   @file   Log.h
 *
 *   @brief  Simple logging infrastructure for Arduino
 *
 ****************************************************************************/

#pragma once

#include <stdarg.h>

#include <cassert>

#include "Str.h"

#if !defined(DISABLE_LOGGING)
//! Define DISABLE_LOGGING to have all traces of logging disappear.
#define DISABLE_LOGGING 0
#endif

//! Define the positive variant, which can be used in constexpr and preprocssor
#define LOGGING_ENABLED (!DISABLE_LOGGING)

//! Abstract Logging class.
class Log {
 public:
    //! Logging levels.
    enum class Level {
        NONE,     //!< Logging is disabled
        FATAL,    //!< Fatal (i.e. asserts)
        ERROR,    //!< Errors
        WARNING,  //!< Warnings
        INFO,     //!< Informational
        DEBUG,    //!< Debug
    };

    //! Constructor.
    Log() {
        assert(logger == nullptr);
        logger = this;
    }

    //! Destructor.
    virtual ~Log() { logger = nullptr; }

    //! Determines if a log level is enabled.
    //! @returns Returns true if `level` should be logged based on the current logging level.
    bool should_log(Level level  //!< [in] Log level to test.
    ) const {
        return level < this->curr_level;
    }

    //! Returns the current logging level.
    //! @returns the current logging level.
    Level get_level() const { return this->curr_level; }

    //! Sets the current logging level.
    void set_level(Level level  //!< [in] Level to set the current logging level to.
    ) {
        this->curr_level = level;
    }

    //! Prints a debug level log.
    static void debug(
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 1, 2))) {
        if constexpr (LOGGING_ENABLED) {
            if (logger != nullptr && logger->should_log(Level::DEBUG)) {
                va_list args;
                va_start(args, fmt);
                logger->do_log(Level::DEBUG, fmt, args);
                va_end(args);
            }
        }
    }

    //! Prints an info level log.
    static void info(
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 1, 2))) {
        if constexpr (LOGGING_ENABLED) {
            if (logger != nullptr && logger->should_log(Level::INFO)) {
                va_list args;
                va_start(args, fmt);
                logger->do_log(Level::INFO, fmt, args);
                va_end(args);
            }
        }
    }

    //! Prints a warning level log.
    static void warning(
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 1, 2))) {
        if constexpr (LOGGING_ENABLED) {
            if (logger != nullptr && logger->should_log(Level::WARNING)) {
                va_list args;
                va_start(args, fmt);
                logger->do_log(Level::INFO, fmt, args);
                va_end(args);
            }
        }
    }

    //! Prints an error level log.
    static void error(
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 1, 2))) {
        if constexpr (LOGGING_ENABLED) {
            if (logger != nullptr && logger->should_log(Level::ERROR)) {
                va_list args;
                va_start(args, fmt);
                logger->do_log(Level::INFO, fmt, args);
                va_end(args);
            }
        }
    }

    //! Prints a fatal level log.
    static void fatal(
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 1, 2))) {
        if constexpr (LOGGING_ENABLED) {
            if (logger != nullptr && logger->should_log(Level::FATAL)) {
                va_list args;
                va_start(args, fmt);
                logger->do_log(Level::INFO, fmt, args);
                va_end(args);
            }
        }
    }

    //! Logs a message of the indicated level using varadic arguments.
    void log(
        Level level,      //!< [in] Level associated with this message.
        const char* fmt,  //!< [in] printf style format string.
        ...               //!< [in] varadic list of parameters
        ) __attribute__((format(printf, 3, 4))) {
        if constexpr (LOGGING_ENABLED) {
            if (this->should_log(level)) {
                va_list args;
                va_start(args, fmt);
                this->vlog(Level::INFO, fmt, args);
                va_end(args);
            }
        }
    }

    //! Logs a message of the indicated level using a va_list/
    void vlog(
        Level level,      //!< [in] Level associated with this message.
        const char* fmt,  //!< [in] printf style format string.
        va_list args      //!< [in] List of parameters
        ) __attribute__((format(printf, 3, 0))) {
        if constexpr (LOGGING_ENABLED) {
            if (this->should_log(level)) {
                this->do_log(level, fmt, args);
            }
        }
    }

 protected:
    //! Function which performs the actual logging.
    virtual void do_log(
        Level level,      //!< [in] Level associated with this message.
        const char* fmt,  //!< [in] printf style format string.
        va_list args      //!< [in] List of parameters
        ) __attribute__((format(printf, 3, 0))) = 0;

    Level curr_level = Level::DEBUG;  //!< Current logging level.
    static Log* logger;               //!< Pointer to the current logger.
};
