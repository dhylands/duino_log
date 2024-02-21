/****************************************************************************
 *
 *   @copyright Public Domain
 *
 *   Since this code originated from code which is public domain, I
 *   hereby declare this code to be public domain as well.
 *
 *   Dave Hylands - dhylands@gmail.com
 *
 ****************************************************************************/
/**
 *  @file    StrPrintf.cpp
 *
 *  @brief   Implementation of a re-entrant printf function.
 *
 *  Implements a reentrant version of the printf function. Also allows a
 *  function pointer to be provided to perform the actual output.
 *
 *  This version of printf was taken from
 *
 *     http://www.efgh.com/software/gprintf.htm
 *
 *  This software was posted by the author as being in the "public" domain.
 *  I've taken the original gprintf.txt and made some minor revisions.
 *
 ****************************************************************************/

/**
 * @defgroup StrPrintf  String Formatting
 * @ingroup  Str
 */
/**
 *  @defgroup StrPrintfInternal   String Formatting Internals
 *  @ingroup  StrPrintf
 */

// ---- Include Files -------------------------------------------------------

#include <limits.h>

#include <cstdint>
#include <cstring>

#include "Str.h"

#if defined(AVR)

#undef StrPrintf
#undef vStrPrintf

#undef StrXPrintf
#undef vStrXPrintf

#define StrPrintf StrPrintf_P
#define vStrPrintf vStrPrintf_P

#define StrXPrintf StrXPrintf_P
#define vStrXPrintf vStrXPrintf_P

#else

//! For compatability with AVR
#define pgm_read_byte(addr) *addr

#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

namespace str {

//! @addtogroup StrPrintfInternal
//!@{

//! Controls a variety of output options.
enum FmtOption : uint8_t {
    NO_OPTION = 0x00,      //!< No options specified.
    MINUS_SIGN = 0x01,     //!< Should we print a minus sign?
    RIGHT_JUSTIFY = 0x02,  //!< Should field be right justified?
    ZERO_PAD = 0x04,       //!< Should field be zero padded?
    CAPITAL_HEX = 0x08,    //!< Did we encounter %X?
    PLUS_SIGN = 0x10,      //!< Should we print a Plus sign?
    SPACE_SIGN = 0x20,     //!< Should we print a space for the sign?
    OUTPUT_BASE = 0x40,    //!< Should we print the base (i.e. 0, 0x, 0b)
};

//! Internal structure which is used to allow vStrXPrintf() to be reentrant.
typedef struct {
    size_t numOutputChars;    //!< Number of characters output so far.
    FmtOption options;        //!< Options determined from parsing format specification.
    int16_t minFieldWidth;    //!< Minimum number of characters to output.
    int16_t editedStringLen;  //!< The exact number of characters to output.
    int16_t leadingZeros;     //!< The number of leading zeros to output.
    StrXPrintfFunc outFunc;   //!< The function to call to perform the actual output.
    void* outParm;            //!< Parameter to pass to the output function.
} Parameters;

//! Determines if an option has been set.
static inline bool IsOptionSet(const Parameters* p, FmtOption x) {
    return (p->options & x) != 0;
}

//! Determines if an option is not set.
static inline bool IsOptionClear(const Parameters* p, FmtOption x) {
    return (p->options & x) == 0;
}

//! Sets an option.
static inline void SetOption(Parameters* p, FmtOption x) {
    p->options = static_cast<FmtOption>(p->options | x);
}

//! Unsets an option.
static inline void ClearOption(Parameters* p, FmtOption x) {
    p->options = static_cast<FmtOption>(p->options & ~x);
}

/**
 * Internal structure used by vStrPrintf() .
 */
typedef struct {
    char* str;  /**< Buffer to store results into.                       */
    int maxLen; /**< Maximum number of characters which can be stored.   */
} StrPrintfParms;

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

static void OutputChar(Parameters* p, char c);
static void OutputField(Parameters* p, char* s, uint16_t base);
static size_t StrPrintfFunc(void* outParm, char ch) noexcept;

//!@}

}  // namespace str

/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup StrPrintf
 * @{
 */

size_t StrPrintf(char* outStr, size_t maxLen, const char* fmt, ...) noexcept {
    int rc;
    va_list args;

    va_start(args, fmt);
    rc = vStrPrintf(outStr, maxLen, fmt, args);
    va_end(args);

    return rc;
}

size_t StrXPrintf(StrXPrintfFunc outFunc, void* outParm, const char* fmt, ...) noexcept {
    int rc;
    va_list args;

    va_start(args, fmt);
    rc = vStrXPrintf(outFunc, outParm, fmt, args);
    va_end(args);

    return rc;
}

size_t vStrPrintf(char* outStr, size_t maxLen, const char* fmt, va_list args) noexcept {
    str::StrPrintfParms strParm;

    strParm.str = outStr;
    strParm.maxLen = maxLen - 1; /* Leave space for temrinating null char   */

    return vStrXPrintf(str::StrPrintfFunc, &strParm, fmt, args);
}

size_t vStrXPrintf(StrXPrintfFunc outFunc, void* outParm, const char* fmt, va_list args) noexcept {
    str::Parameters p;
    char controlChar;

    p.numOutputChars = 0;
    p.outFunc = outFunc;
    p.outParm = outParm;

    controlChar = pgm_read_byte(fmt++);

    while (controlChar != '\0') {
        if (controlChar == '%') {
            int16_t precision = -1;
            int16_t longArg = 0;
            int16_t longLongArg = 0;
            int16_t base = 0;

            controlChar = pgm_read_byte(fmt++);
            p.minFieldWidth = 0;
            p.leadingZeros = 0;
            p.options = str::FmtOption::NO_OPTION;

            SetOption(&p, str::FmtOption::RIGHT_JUSTIFY);

            // Process [flags]

            if (controlChar == '-') {
                ClearOption(&p, str::FmtOption::RIGHT_JUSTIFY);
                controlChar = pgm_read_byte(fmt++);
            }
            if (controlChar == '+') {
                SetOption(&p, str::FmtOption::PLUS_SIGN);
                controlChar = pgm_read_byte(fmt++);
            } else if (controlChar == ' ') {
                SetOption(&p, str::FmtOption::SPACE_SIGN);
                controlChar = pgm_read_byte(fmt++);
            } else if (controlChar == '#') {
                SetOption(&p, str::FmtOption::OUTPUT_BASE);
                controlChar = pgm_read_byte(fmt++);
            }

            if (controlChar == '0') {
                SetOption(&p, str::FmtOption::ZERO_PAD);
                controlChar = pgm_read_byte(fmt++);
            }

            // Process [width]

            if (controlChar == '*') {
                p.minFieldWidth = (int16_t)va_arg(args, int);
                controlChar = pgm_read_byte(fmt++);
            } else {
                while (('0' <= controlChar) && (controlChar <= '9')) {
                    p.minFieldWidth = p.minFieldWidth * 10 + controlChar - '0';
                    controlChar = pgm_read_byte(fmt++);
                }
            }

            // Process [.precision]

            if (controlChar == '.') {
                controlChar = pgm_read_byte(fmt++);
                if (controlChar == '*') {
                    precision = (int16_t)va_arg(args, int);
                    controlChar = pgm_read_byte(fmt++);
                } else {
                    precision = 0;
                    while (('0' <= controlChar) && (controlChar <= '9')) {
                        precision = precision * 10 + controlChar - '0';
                        controlChar = pgm_read_byte(fmt++);
                    }
                }
            }

            // Process [l]

            if (controlChar == 'l') {
                longArg = 1;
                controlChar = pgm_read_byte(fmt++);
                if (controlChar == 'l') {
                    longLongArg = 1;
                    controlChar = pgm_read_byte(fmt++);
                }
            }

            if (controlChar == 'h') {
                // For %hu (unsigned short) and %hhu (unsigned char), the value is promoted to an
                // int, so We can essentially ignore the h/hh portion.
                controlChar = pgm_read_byte(fmt++);
                if (controlChar == 'h') {
                    controlChar = pgm_read_byte(fmt++);
                }
            }

            // Process type.

            if (controlChar == 'd' || controlChar == 'i') {
                controlChar = 'd';
                base = 10;
            } else if (controlChar == 'x') {
                base = 16;
            } else if (controlChar == 'X') {
                base = 16;
                SetOption(&p, str::FmtOption::CAPITAL_HEX);
            } else if (controlChar == 'u') {
                base = 10;
            } else if (controlChar == 'o') {
                base = 8;
            } else if (controlChar == 'b') {
                base = 2;
            } else if (controlChar == 'c') {
                base = -1;
                ClearOption(&p, str::FmtOption::ZERO_PAD);
            } else if (controlChar == 's') {
                base = -2;
                ClearOption(&p, str::FmtOption::ZERO_PAD);
            }

            if (base == 0) { /* invalid conversion type */
                if (controlChar != '\0') {
                    str::OutputChar(&p, '%');
                    str::OutputChar(&p, controlChar);
                    controlChar = pgm_read_byte(fmt++);
                }
            } else {
                if (base == -1) { /* conversion type c */
                    char c = (char)va_arg(args, int);
                    p.editedStringLen = 1;
                    OutputField(&p, &c, 10);
                } else if (base == -2) { /* conversion type s */
                    char* string = va_arg(args, char*);

                    p.editedStringLen = 0;
                    while (string[p.editedStringLen] != '\0') {
                        if ((precision >= 0) && (p.editedStringLen >= precision)) {
                            /*
                             * We don't require the string to be null terminated
                             * if a precision is specified.
                             */

                            break;
                        }
                        p.editedStringLen++;
                    }
                    OutputField(&p, string, 10);
                } else {                   /* conversion type d, b, o or x */
                    unsigned long long x;  // NOLINT

                    /*
                     * Worst case buffer allocation is required for binary output,
                     * which requires one character per bit of a long.
                     */

                    char buffer[CHAR_BIT * sizeof(unsigned long long) + 1];  // NOLINT

                    p.editedStringLen = 0;
                    if (longLongArg) {
                        x = va_arg(args, unsigned long long);  // NOLINT
                    } else if (longArg) {
                        x = va_arg(args, unsigned long);  // NOLINT
                    } else if (controlChar == 'd') {
                        x = va_arg(args, int);
                    } else {
                        x = va_arg(args, unsigned);
                    }

                    if ((controlChar == 'd') && ((long long)x < 0)) {  // NOLINT
                        SetOption(&p, str::FmtOption::MINUS_SIGN);
                        ClearOption(&p, str::FmtOption::PLUS_SIGN);
                        x = -(long long)x;  // NOLINT
                    }

                    do {
                        int c;
                        c = x % base + '0';
                        if (c > '9') {
                            if (IsOptionSet(&p, str::FmtOption::CAPITAL_HEX)) {
                                c += 'A' - '9' - 1;
                            } else {
                                c += 'a' - '9' - 1;
                            }
                        }
                        buffer[sizeof(buffer) - 1 - p.editedStringLen++] = (char)c;
                    } while ((x /= base) != 0);

                    if ((precision >= 0) && (precision > p.editedStringLen)) {
                        p.leadingZeros = precision - p.editedStringLen;
                    }
                    OutputField(&p, buffer + sizeof(buffer) - p.editedStringLen, base);
                }
                controlChar = pgm_read_byte(fmt++);
            }
        } else {
            // We're not processing a % output. Just output the character that was encountered.

            str::OutputChar(&p, controlChar);
            controlChar = pgm_read_byte(fmt++);
        }
    }
    return p.numOutputChars;
}

//!@}

/**
 * @addtogroup StrPrintfInternal
 * @{
 */

/***************************************************************************/
/**
 *  Outputs a single character, keeping track of how many characters have
 * been output.
 *
 *  @param   p     (mod) State information.
 *  @param   c     (in)  Character to output.
 */

static void str::OutputChar(Parameters* p, char c) {
    int n = (*p->outFunc)(p->outParm, c);

    if (n > 0) {
        p->numOutputChars++;
    }
}

/***************************************************************************/
/**
 *  Outputs a formatted field. This routine assumes that the field has been
 *  converted to a string, and this routine takes care of the width
 *  options, leading zeros, and any leading minus sign.
 *
 *  @param   p     (mod) State information.
 *  @param   s     (in)  String to output.
 */

static void str::OutputField(Parameters* p, char* s, uint16_t base) {
    int16_t padLen = p->minFieldWidth - p->leadingZeros - p->editedStringLen;

    if (IsOptionSet(p, MINUS_SIGN)) {
        if (IsOptionSet(p, ZERO_PAD)) {
            // Since we're zero padding, output the minus sign now. If we're space
            // padding, we wait until we've output the spaces.
            OutputChar(p, '-');
        }

        // Account for the minus sign now, even if we are going to output it
        // later. Otherwise we'll output too much space padding.
        padLen--;
    } else if (IsOptionSet(p, PLUS_SIGN)) {
        if (IsOptionSet(p, ZERO_PAD)) {
            // Since we're zero padding, output the minus sign now. If we're space
            // padding, we wait until we've output the spaces.
            OutputChar(p, '+');
        }

        // Account for the minus sign now, even if we are going to output it
        // later. Otherwise we'll output too much space padding.
        padLen--;
    } else if (IsOptionSet(p, SPACE_SIGN)) {
        if (IsOptionSet(p, ZERO_PAD)) {
            // Since we're zero padding, output the minus sign now. If we're space
            // padding, we wait until we've output the spaces.
            OutputChar(p, ' ');
        }

        // Account for the minus sign now, even if we are going to output it
        // later. Otherwise we'll output too much space padding.
        padLen--;
    } else if (IsOptionSet(p, OUTPUT_BASE) && *s != '0') {
        // printf doesn't output the base if the value is 0
        // Since we're zero padding, output the prefix now.
        if (base == 16) {
            if (IsOptionSet(p, ZERO_PAD)) {
                OutputChar(p, '0');
                if (IsOptionSet(p, str::FmtOption::CAPITAL_HEX)) {
                    OutputChar(p, 'X');
                } else {
                    OutputChar(p, 'x');
                }
            }
            padLen -= 2;
        } else if (base == 8) {
            if (IsOptionSet(p, ZERO_PAD)) {
                OutputChar(p, '0');
            }
            p->leadingZeros--;
            padLen--;
        } else if (base == 2) {
            if (IsOptionSet(p, ZERO_PAD)) {
                OutputChar(p, '0');
                OutputChar(p, 'b');
            }
            padLen -= 2;
        }
    }

    if (IsOptionSet(p, RIGHT_JUSTIFY)) {
        /*
         * Right justified: Output the spaces then the field.
         */

        while (--padLen >= 0) {
            OutputChar(p, p->options & ZERO_PAD ? '0' : ' ');
        }
    }
    if (IsOptionClear(p, ZERO_PAD)) {
        if (IsOptionSet(p, MINUS_SIGN)) {
            // We're not zero padding, which means we haven't output the minus
            // sign yet. Do it now.
            OutputChar(p, '-');
        } else if (IsOptionSet(p, PLUS_SIGN)) {
            // We're not zero padding, which means we haven't output the plus
            // sign yet. Do it now.
            OutputChar(p, '+');
        } else if (IsOptionSet(p, SPACE_SIGN)) {
            // We're not zero padding, which means we haven't output the plus
            // sign yet. Do it now.
            OutputChar(p, ' ');
        } else if (IsOptionSet(p, OUTPUT_BASE) && *s != '0') {
            // printf doesn't output the base if the value is 0.
            if (base == 16) {
                OutputChar(p, '0');
                if (IsOptionSet(p, str::FmtOption::CAPITAL_HEX)) {
                    OutputChar(p, 'X');
                } else {
                    OutputChar(p, 'x');
                }
            } else if (base == 8) {
                OutputChar(p, '0');
            } else if (base == 2) {
                OutputChar(p, '0');
                OutputChar(p, 'b');
            }
        }
    }

    /*
     * Output any leading zeros.
     */

    while (--p->leadingZeros >= 0) {
        OutputChar(p, '0');
    }

    /*
     * Output the field itself.
     */

    while (--p->editedStringLen >= 0) {
        OutputChar(p, *s++);
    }

    /*
     * Output any trailing space padding. Note that if we output leading
     * padding, then padLen will already have been decremented to zero.
     */

    while (--padLen >= 0) {
        OutputChar(p, ' ');
    }
}

/***************************************************************************/
/**
 *  Helper function, used by vStrPrintf() (and indirectly by StrPrintf())
 *  for outputting characters into a user supplied buffer.
 *
 *  @param   outParm  (mod) Pointer to StrPrintfParms structure.
 *  @param   ch       (in)  Character to output.
 *
 *  @return  1 if the character was stored successfully, -1 if the buffer
 *           was overflowed.
 */

static size_t str::StrPrintfFunc(void* outParm, char ch) noexcept {
    str::StrPrintfParms* strParm = reinterpret_cast<str::StrPrintfParms*>(outParm);

    if (strParm->maxLen > 0) {
        *strParm->str++ = (char)ch;
        *strParm->str = '\0';
        strParm->maxLen--;

        return 1;
    }

    // Whoops. We ran out of space.
    return 0;
}

// NOTE: For some reason, doxygen considers these to be different from the Prototypes
//       in the Str.h file, so we need to repeat the documentation part. I suspect
//       it has something to do with using an attribute alias.

//! Alias functions which don't do attribute checking.
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
    ) noexcept __attribute__((alias("StrPrintf")));
size_t vStrBPrintf(
    char* outStr,     //!< [out] Place to store formatted string.
    size_t maxLen,    //!< [in] Length out `outStr`.
    const char* fmt,  //!< [in] Printf style format string.
    va_list args      //!< [in] Arguments associated with the format string.
    ) noexcept __attribute__((alias("vStrPrintf")));
size_t StrXBPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    ...                   //!< [in] Varadic arguments associated with format string.
    ) noexcept __attribute__((alias("StrXPrintf")));
size_t vStrXBPrintf(
    StrXPrintfFunc func,  //!< [in] Function to be called for each character to output.
    void* userParm,       //!< [in] Context passed to func().
    const char* fmt,      //!< [in] Printf style format string.
    va_list args          //!< [in] Arguments associated with the format string.
    ) noexcept __attribute__((alias("vStrXPrintf")));
//! @}

//! @}
