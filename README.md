# The display driver plugin for LCD Smartie
[![Build Status](https://travis-ci.org/eeyrw/LcdTcpDll.svg?branch=master)](https://travis-ci.org/eeyrw/LcdTcpDll) [![GitHub last commit](https://img.shields.io/github/last-commit/google/skia.svg)]() [![GitHub version](https://badge.fury.io/gh/eeyrw%2FLcdTcpDll.svg)](https://badge.fury.io/gh/eeyrw%2FLcdTcpDll)

This driver plugin is a native DLL under Windows. which support the display through network ( such as Ethernet, WiFi, etc ). It works with ESP8266 based device ( https://github.com/eeyrw/LcdTcp ) by now.
## Build the plugin
If you are not about to build plugin, you can just download the release from GitHub [release page](https://github.com/eeyrw/LcdTcpDll/releases).

You can build it with MSVC or mingw32. I create a project file with IDE CodeBlocks. When you use MSVC, you should specify the .def file to generate correct DLL. When you use mingw, you should add -m32 to compiler flag and -static to linker flag. Because I discover that if you do not attach -static, mingw will dynamically link it's libgccXXX.dll, which is not expected.  
## Use the plugin

It's fairly simple. Copy the dll to `LCD_SMARTIE_ROOT\displays` and set IP address and port of your display.
![LCD Smartie setting demo](LCD%20Smartie%20setting%20demo.png)

If you are failed to run LCD Smartie you could edit `LCD_SMARTIE_ROOT\config.ini` and append following lines to category `[Communication Settings]`:

    DisplayDLLName=LCD_TCP_MINGW_DLL.dll
    DisplayDLLParameters=192.168.1.134:2400

<!--stackedit_data:
eyJoaXN0b3J5IjpbMjA1NDMwMzk5NF19
-->