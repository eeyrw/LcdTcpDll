#!/bin/sh
i686-w64-mingw32-gcc -O2 -Wall -DBUILD_DLL  -c debug_output.c -o debug_output.o
i686-w64-mingw32-gcc -O2 -Wall -DBUILD_DLL  -c LCDS_Driver.c -o LCDS_Driver.o
i686-w64-mingw32-gcc -O2 -Wall -DBUILD_DLL  -c tcp.c -o tcp.o
i686-w64-mingw32-g++ -shared   -Wl,--dll  debug_output.o LCDS_Driver.o tcp.o  -o LCD_TCP_MINGW_DLL.dll -static -s -Wl,--kill-at  -luser32 -lws2_32 -luser32 -lws2_32
