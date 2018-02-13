# The display driver plugin for LCD Smartie

This driver plugin is a DLL under Windows. which support the display through network ( such as Ethernet, WiFi, etc ). It works with ESP8266 based device ( https://github.com/eeyrw/LcdTcp ) by now.
## Build the plugin
You may build it with MSVC or mingw32. I create a project file with IDE CodeBlocks. When you use MSVC, you should specify the .def file to generate correct DLL. When you use mingw, you should add -m32 to compiler flag and -static to linker flag. Because I discovered that if you do not attach -static. mingw will dynamically link it's libgccXXX.dll, which is not expected.  
## Use the plugin
It's fairly simple. Copy the dll to `LCD_SMARTIE_ROOT\displays` and set IP address and port of your display.
![LCD Smartie setting demo](https://picasaweb.google.com/107384983838237380474/6521878153860292481#6521878154250877698 "LCD Smartie setting demo")
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTAzOTQ3OTQ5NF19
-->