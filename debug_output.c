#include "debug.h"

#ifdef DEBUG_LEVEL_1_EN||DEBUG_LEVEL_2_EN

void WINAPI DebugString(LPCSTR format, ...)
{
    va_list arglist;
    char buffer[1024];
    va_start (arglist,format);
    vsprintf(buffer, format, arglist);
    va_end (arglist);
    strcat(buffer, "\n");
    OutputDebugString (buffer);
}
#endif
