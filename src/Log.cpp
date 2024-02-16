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

#include "Log.h"

#if LOGGING_ENABLED
//! Pointer to the global logger object.
Log* Log::logger = nullptr;
#endif  // LOGGING_ENABLED
