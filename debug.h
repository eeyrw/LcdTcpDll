#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <windows.h>
#include <stdio.h>

#define DEBUG_LEVEL_1_EN
//#define DEBUG_LEVEL_2_EN

#ifdef DEBUG_LEVEL_1_EN
#define d1printf DebugString
#else
#define d1printf(...)
#endif

#ifdef DEBUG_LEVEL_2_EN
#define d2printf DebugString
#else
#define d2printf(...)
#endif




void WINAPI DebugString(LPCSTR format, ...);


#endif
