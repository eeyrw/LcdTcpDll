#include <stdio.h>
#include "LCDS_Driver.H"
#include <windows.h>
#include <stdlib.h>
#include "debug.h"
#include "tcp.h"

typedef struct _LCD_INIT_DATA
{
    char paramStr[512];
    int sizeX;
    int sizeY;
    int curosrX;
    int curosrY;
    char customChar[8][8];
    char currentLcdContent[4][40];
    unsigned char backLightOnOff;
    unsigned char contrastLevel;
    unsigned char brightnessLevel;



} LCD_INIT_DATA;

VOID CALLBACK DeamonProc(
    HWND hWnd, // handle of window for timer messages
    UINT uMsg, // WM_TIMER message
    UINT idEvent, // timer identifier
    DWORD dwTime // current system time
);

static LCD_INIT_DATA gLcdInitData;

static char result_str[80]="";
static unsigned char Buf[256];


int globalConnStatus=-1;
int IP_Array[4];
uint32_t u32_IP;
uint32_t portNum;



// driver name
#define DefaultParameters "192.168.1.134:2400"

#define DRIVER_NAME "LCD Smartie DLL TCP mDNS mingw"

// usage
#define USAGE "IP address like:192.168.1.134:2400"


#define CMD_LCD_INIT 0x01
#define CMD_LCD_SETBACKLIGHT 0x02
#define CMD_LCD_SETCONTRAST 0x03
#define CMD_LCD_SETBRIGHTNESS 0x04
#define CMD_LCD_WRITEDATA 0x05
#define CMD_LCD_SETCURSOR 0x06
#define CMD_LCD_CUSTOMCHAR 0x07
#define CMD_LCD_WRITECMD 0x08
#define CMD_ECHO 0x09
#define CMD_GET_VER_INFO 0x0A
#define CMD_LCD_DE_INIT 0x0B
#define CMD_ENTER_BOOT 0x19

#define OFFEST_CMD_DATA 3


uint8_t* GetCmdDataPtr(void)
{
    return (uint8_t*)(&Buf[OFFEST_CMD_DATA]);
}




BOOL SendDataTcp(uint32_t len)
{
    int status=-1;
    if((len<2)||(len>64))
        return -1;

    if(globalConnStatus)
    {
        status=TcpSendData((char*)Buf,len);
    }
    if(status<0)
    {
        globalConnStatus=-1;
    }

    return status;
}

int SendCmdData(uint32_t cmdDataLen)
{
    Buf[0]='n';
    Buf[1]='w';
    Buf[2]=cmdDataLen;
    return SendDataTcp(cmdDataLen+OFFEST_CMD_DATA);
}

//Function: DISPLAYDLL_Init
//
//Parameters: SizeX,SizeY : byte; StartupParameters : pchar; OK : pintean;
//
//Result: none
//
//Function description: First function called when LCD Smartie starts a display
//or comes back from standby. First two parameters are bytes that describe the
//size that LCD Smartie believes the display to be, the third parameter are the
//startup parameters as a null terminated string, and the fourth parameter is a
//pointer to a intean that the DLL can use to pass back success or failure of
//DLL startup. (For example, if the calling application, LCD Smartie, wants the
//driver to start a COM port that doesn't exist, pass back -1 to indicate the
//start of the COM port failed).

DLL_EXPORT(char *) DISPLAYDLL_Init(LCDS_BYTE size_x,LCDS_BYTE size_y,char *startup_parameters,LCDS_BOOL *ok)
{


    BOOL Result=1;

    strcpy(gLcdInitData.paramStr, startup_parameters);
    sscanf(startup_parameters,"%d.%d.%d.%d:%ud",&IP_Array[0],&IP_Array[1],&IP_Array[2],&IP_Array[3],&portNum);
    u32_IP=GetIP_U32(IP_Array[0],IP_Array[1],IP_Array[2],IP_Array[3]);

    uint32_t u32_IP_resolved;
    if(ResolveHostName("WIFI_LCD_STA.local",&u32_IP_resolved))
    {
        u32_IP=u32_IP_resolved;
    }
    if (TcpInit(u32_IP,portNum)<=0) //initalize winsocks
    {
        Result=-1; //failed
    }





    d1printf("The Init para:%s",startup_parameters);

    if(Result==-1)
    {
        strcpy(result_str, "Fail to initialize tcp lcd device.");
        d1printf("Fail to initialize tcp lcd device.");
    }
    else
    {
        SetTimer(NULL, 0, 3000, (TIMERPROC)DeamonProc);
        globalConnStatus=1;
        gLcdInitData.sizeY=size_y;
        gLcdInitData.sizeX=size_x;

        uint8_t* cmdBuf=GetCmdDataPtr();
        cmdBuf[0]=CMD_LCD_INIT;
        cmdBuf[1]=gLcdInitData.sizeX;
        cmdBuf[2]=gLcdInitData.sizeY;
        Sleep(100);
        SendCmdData(3);
        *ok=TRUE;

        d1printf("Success to open device!");
        d1printf("LCD size:x=%d,y=%d.",gLcdInitData.sizeX,gLcdInitData.sizeY);

    }


    return result_str;
}

// +-----------------------+
// ! DISPLAYDLL_DriverName !
// +-----------------------+
//! Returns the driver name.
//! The driver name is used by the driver selection GUI to identify the driver.
//! It's good practice to return a proper name and version for the driver so that
//! it can be clearly identified (or else we would have to rely on the plugin name
//! which is subject to modifications).
DLL_EXPORT(char *) DISPLAYDLL_DriverName(void)
{
    d1printf("Call DISPLAYDLL_DriverName");
    return DRIVER_NAME;
}

//Function: DISPLAYDLL_Usage
//
//Parameters: none
//
//Results: Usage : pchar;
//
//Cunction description: Returns plugin usage hints.
//The usage text describes the main plugin parameters and how they are assembled
//in the parameter string (syntax indications).

DLL_EXPORT(char *) DISPLAYDLL_Usage(void)
{
    d1printf("Call DISPLAYDLL_Usage");
    return USAGE;
}


//Function: DISPLAYDLL_DefaultParameters
//
//Parameters: none
//
//Results: DefaultParameters : pchar;
//
//Function description: Returns the plugin default parameters.
//The default parameters string is used as parameter string after the driver
//has been initialized.

DLL_EXPORT(char *) DISPLAYDLL_DefaultParameters(void)
{

    d1printf("Call DISPLAYDLL_ReadKey");
    return DefaultParameters;
}

//Function: DISPLAYDLL_SetPosition
//
//Parameters: X,Y : byte
//
//Result: none
//
//Function description: Tells LCD where to position the cursor for the next
//display command. For example, when LCD Smartie wants to update the second
//line on the display, it will call this command with 1,2 parameters. The
//coordinates are "1" based, meaning the upper left corner is 1,1.

DLL_EXPORT(void) DISPLAYDLL_SetPosition(LCDS_BYTE x,LCDS_BYTE y)
{
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_SETCURSOR;
    cmdBuf[1]=x-1;
    cmdBuf[2]=y-1;
    gLcdInitData.curosrX=x;
    gLcdInitData.curosrY=y;
    SendCmdData(3);
    d1printf("SetPosition with x=%d,y=%d!",gLcdInitData.curosrX-1,gLcdInitData.curosrY-1);
}

//Function: DISPLAYDLL_Write
//
//Parameters: Str : pchar
//
//Result: none
//
//Function description: Write the null terminated string at the current
//cursor position. This is the primary function LCD Smartie uses to write to
//the display.

DLL_EXPORT(void) DISPLAYDLL_Write(char *str)
{
    int i;

    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_WRITEDATA;

    for(i=0; i<gLcdInitData.sizeX; i++)
    {


        //if((*str)==0)
        //	break;
        gLcdInitData.currentLcdContent[gLcdInitData.curosrY-1][i+gLcdInitData.curosrX-1]=(*str);
        cmdBuf[i+2]=(*str);


        //switch (Buf[i+2]) {
        //case 176: Buf[i+2] = 0; break;
        //case 158: Buf[i+2] = 1; break;
        //case 131: Buf[i+2] = 2; break;
        //case 132: Buf[i+2] = 3; break;
        //case 133: Buf[i+2] = 4; break;
        //case 134: Buf[i+2] = 5; break;
        //case 135: Buf[i+2] = 6; break;
        //case 136: Buf[i+2] = 7; break;
        //default: break;
        //}

        str++;

    }
    d1printf("WriteData with %s",&cmdBuf[2]);
    cmdBuf[1]=i;
    SendCmdData(3+gLcdInitData.sizeX);


}

//Function: DISPLAYDLL_SetBrightness
//
//Parameters: Brightness : byte
//
//Result: none
//
//Function description: First parameter tells display DLL what brightness value
//LCD Smartie wants, as set by the user in the setup screen. Default value is
//128, values range from 0-255. 0 is dim, 255 is max brightness. Typically
//only used on VFD displays.

DLL_EXPORT(void) DISPLAYDLL_SetBrightness(LCDS_BYTE brightness)
{

    d1printf("SetBrightness with %d!",brightness);
    gLcdInitData.brightnessLevel=brightness;
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_SETBRIGHTNESS;
    cmdBuf[1]=brightness;
    SendCmdData(2);
}

//Function: DISPLAYDLL_CustomChar
//
//Parameters: Chr : byte; Data : TCustomArray
//
//Result: none
//
//Function description: Defines custom char (CGRAM) for the display. The
//first parameter is which character is being defined. The value is 1 to 8.
//The next parameter is an array of 8 bytes that define the custom character.
//Please note that when defining custom characters, only the least significant
//five bits are effective (5x8 char).

DLL_EXPORT(void) DISPLAYDLL_CustomChar(LCDS_BYTE chr,LCDS_BYTE *data)
{
    int i;
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_CUSTOMCHAR;
    cmdBuf[1]=chr-1;
    for(i=0; i<8; i++)
    {

        gLcdInitData.customChar[cmdBuf[1]][i]=(*data);
        cmdBuf[i+2]=(*data);
        data++;
    }
    SendCmdData(10);

}

//Function: DISPLAYDLL_CustomCharIndex
//
//Parameters: Index : byte
//
//Result: 8-bit byte
//
//Function description: This allows LCD Smartie to preprocess the write data
//when it contains custom (CGRAM) characters to contain the correct characters.
//Historically, LCD Smartie used 176,158,131,132,133,134,135,136 for the CGRAM
//characters because it allowed the on screen font to display certain characters.
//Most LCD displays use either 0-7 (with a duplicate at 8-15) or 128-135. When
//LCD Smartie starts the display, it will ask for what values to use by passing
//in the Index (1-8) and using the 8 bit result from this function to substitute
//for the default CGRAM characters. Note that if the LCD display uses 0-7, with
//an 8-15 overlap, 8-15 should be used, since 0 will terminate the null string
//passed in the write procedure. Not overriding this function will result in
//LCD Smartie passing the default 176,158,131,132,133,134,135,136 characters for
//custom CGRAM characters.

DLL_EXPORT(LCDS_BYTE) DISPLAYDLL_CustomCharIndex(LCDS_BYTE index)
{

    d1printf("Call DISPLAYDLL_CustomCharIndex£º%d",index);
    --index;
    //if (0==index) index=8;
    return index;
}

//Function: DISPLAYDLL_Done
//
//Parameters: None
//
//Result: none
//
//Function description: Called when LCD Smartie closes the display to enter
//standby or close the application

DLL_EXPORT(void) DISPLAYDLL_Done(void)
{
    d1printf("Call DISPLAYDLL_Done!");
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_DE_INIT;
    SendCmdData(1);

}

//Function: DISPLAYDLL_ReadKey
//
//Parameters: none
//
//Result: 16-bit word
//
//Function description: This function attempts to read a keypad attached to the
//LCD. This is a polled function, not an interrupt driven function, so it will
//be up to the DLL to resolve multiple keypresses in a polling interval. Pass
//back a failed keyscan (no key is pressed) by setting the high byte to 0xFF.
//If the MSB is zero, LCD Smartie will assume the keyscan was successful, and
//the data in the low byte will indicate the key.

DLL_EXPORT(LCDS_WORD) DISPLAYDLL_ReadKey(void)
{
    d1printf("Call DISPLAYDLL_ReadKey");
    return 0;
}

//Function: DISPLAYDLL_SetBacklight
//
//Parameters: LightOn : intean
//
//Result: none
//
//Function description: First parameter is an 8 bit intean that tells the DLL
//whether to turn the backlighting on or off.

DLL_EXPORT(void) DISPLAYDLL_SetBacklight(LCDS_BOOL light_on)
{

    d1printf("SetBacklight with %d!",light_on);
    gLcdInitData.backLightOnOff=light_on;
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_SETBACKLIGHT;
    cmdBuf[1]=light_on;
    SendCmdData(2);
}

//Function: DISPLAYDLL_SetContrast
//
//Parameters: Contrast : byte
//
//Result: none
//
//Function description: First parameter tells display DLL what contrast value
//LCD Smartie wants, as set by the user in the setup screen. Default value is
//128, values range from 0-255. Typically only used on LCD displays.

DLL_EXPORT(void) DISPLAYDLL_SetContrast(LCDS_BYTE contrast)
{

    d1printf("SetContrast with %d!",contrast);
    gLcdInitData.contrastLevel=contrast;
    uint8_t* cmdBuf=GetCmdDataPtr();
    cmdBuf[0]=CMD_LCD_SETCONTRAST;
    cmdBuf[1]=contrast;
    SendCmdData(2);
}


//Function: DISPLAYDLL_PowerResume
//
//Parameters: none
//
//Result: none
//
//Function description: Called when power resumes from standby. NOTE: This
//function is currently NOT implemented in LCD Smartie, and is there for future
//expansion only.

DLL_EXPORT(void) DISPLAYDLL_PowerResume(void)
{
}


//Function: DISPLAYDLL_SetGPO
//
//Parameters: GPO : byte; GPOOn : intean
//
//Result: none
//
//Function description: Updates General Purpose Output (GPO). Supported by
//Matrix Orbital displays. First parameter is which GPO to set, second
//parameter sets on or off state.

DLL_EXPORT(void) DISPLAYDLL_SetGPO(LCDS_BYTE gpo,LCDS_BOOL gpo_on)
{

}

//Function: DISPLAYDLL_SetFan
//
//Parameters: T1,T2 : byte
//
//Result: none
//
//Function description: Updates fan controller. Supported by Matrix Orbital
//displays.

DLL_EXPORT(void) DISPLAYDLL_SetFan(LCDS_BYTE t1,LCDS_BYTE t2)
{
    d1printf("Call DISPLAYDLL_ReadKey");
}

int ReInitLcd(void)
{
    LCDS_BOOL res=-1;
    DISPLAYDLL_Init(gLcdInitData.sizeX,gLcdInitData.sizeY,gLcdInitData.paramStr,&res);
    if(!res)
    {
        return -1;
    }
    for(int i=0; i<8; i++)
    {
        DISPLAYDLL_CustomChar(i+1,(LCDS_BYTE*)&gLcdInitData.customChar[i][0]);
    }

    for(int i=0; i<gLcdInitData.sizeY; i++)
    {
        DISPLAYDLL_SetPosition(1,i+1);
        DISPLAYDLL_Write(&gLcdInitData.currentLcdContent[i][0]);
    }

    DISPLAYDLL_SetContrast(gLcdInitData.contrastLevel);
    DISPLAYDLL_SetBacklight(gLcdInitData.backLightOnOff);
    DISPLAYDLL_SetBrightness(gLcdInitData.brightnessLevel);


    return 1;

}

VOID CALLBACK DeamonProc(
    HWND hWnd, // handle of window for timer messages
    UINT uMsg, // WM_TIMER message
    UINT idEvent, // timer identifier
    DWORD dwTime // current system time
)
{
    if(globalConnStatus!=TRUE)
    {
        if(ReInitLcd())
        {
            globalConnStatus=TRUE;
        }
    }
    return;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
)
{
    switch(fdwReason)
    {
    case 1:
        d1printf("DLL_PROCESS_ATTACH");
        break;
    case 0:
        d1printf("DLL_PROCESS_DETACH");
        break;
    case 2:
        d1printf("DLL_THREAD_ATTACH");
        break;
    case 3:
        d1printf("DLL_THREAD_DETACH");
        break;
    }

    return TRUE;

}


