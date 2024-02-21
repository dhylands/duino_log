# DuinoLog
A simple logging facility.

[TOC]

There are 5 major submodules provided by the DuinoLog library:
- Log
- DumpMem
- Str
- StrPrintf
- Util

## Log

The `Log` class provides an interface for outputting log messages.
The actual output can be overrided. There is an example which
outputs to the Arduino Serial device, and another example which
provides colorized output to the Arduino Serial device.

File based logging could be easily implemented as well.

## DumpMem

DumpMemLine() and DumpMem() are useful functions for printing out
binary data. For example:
```
static uint8_t data[] = {
    0x00, 0x01, 0x02, 0x31, 0x32, 0x33, 0x41, 0x42,
    0x43, 0x11, 0x12, 0x13, 0x36, 0x37, 0x38, 0x39,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x61, 0x62,
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
};

DumpMem("Test", 0, data, 32);
```
would log the following output:
```
Test: 0000: 00 01 02 31 32 33 41 42 43 11 12 13 36 37 38 39 ...123ABC...6789
Test: 0010: 20 21 22 23 24 25 61 62 63 64 65 66 67 68 69 6a  !\"#$%abcdefghij
```

## Str

Two bounded functions, StrMaxCpy() and StrMaxCat() are provided.

StrMaxCpy will always produce a null terminated output (where strncpy doesn't do null termination if the output buffer is too small).

StrMaxCat() is also bounded, where the bound is the length of the output
buffer, where with strncat, the bound is on the length of the input buffer.

## StrPrintf

A simple printf implementation is provided. It only implements an equivalent for snprintf. StrPrintf is basically an equivalent for snprintf with slightly different semantics on the return vaue.

StrPrintf returns the number of characters output, where snprintf
returns the number of characters which would have been output if there
were no length limit.

The StrPrintf behavior is useful when concatenating severl StrPrintf variants like the following:

```
char output[80];

auto len = StrPrintf(output, LEN(output), "%s", var1);
if (some-condition) {
    len += StrPrintf(&output[len], LEN(output) - len, " %d%, var2);
}
```
With snprintf, if the first call were to overflow the buffer, the second
call would write beyond the end of `output` (in effect becoming a buffer overflow). With StrPrintf, the output buffer will not overflow.

StrXPrintf allows a character output function to be provided, and that
function will be called to output each character. This makes implementing
a printf like function to your character LCD quite straight forward.

## Util

The Util.h file provides a template function called LEN which returns the
number of elements in an array. So you can do the following:
```
int array[] = {1, 2, 3};
```
and `LEN(array)` will evaluate to `3`. The LEN template function will
generate a compiler error if passed a bare pointer rather than an array.