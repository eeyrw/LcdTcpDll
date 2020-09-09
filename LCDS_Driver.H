#ifndef LCDS_DRIVER_H
#define LCDS_DRIVER_H

#include <stdint.h>

#ifndef DLL_EXPORT
#define DLL_EXPORT(type) __declspec(dllexport) type __stdcall
#endif

typedef uint8_t LCDS_BYTE;
typedef uint16_t LCDS_WORD;
typedef uint8_t LCDS_BOOL;

#endif
