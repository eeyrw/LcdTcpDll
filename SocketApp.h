#ifndef __SOCKETAPP_H__
#define __SOCKETAPP_H__

#include <iostream>
#include <cstdio>
#include <windows.h>
#include "Socket.h"
#include <string>
#include <cstdarg>
#include "debug.h"

using namespace std;

// The time in seconds after which a client disconnects from his idle server
// Set 0 to disable the timeout
#define MAX_CLIENT_IDLE_TIME  0 // off

string format(const string& fmt, ...);
string GetErrMsg(DWORD u32_Error);
string FormatIP (DWORD u32_IP);
string FormatDisplayName(SOCKET h_Socket);
void GetLocalIP(void);
 DWORD GetIP_U32(int IP0,int IP1,int IP2,int IP3);
  void TCP_Client_ProcessEvents();
  void CloseSockets();
  void ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem);
  bool Connect(DWORD TargetIP,DWORD TargetPort);
  BOOL SendToServer(uint8_t* Buf,uint32_t u32_Len);

static ULONG WINAPI ProcessEventThread(void* p_Param);

#endif // __SOCKETAPP_H__
