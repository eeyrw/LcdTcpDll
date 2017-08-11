#ifndef UDP_H_INCLUDED
#define UDP_H_INCLUDED

#include <stdio.h>
#include <stdint.h>


#pragma comment(lib, "ws2_32.lib")

int UdpInit(uint32_t u32Ip,uint32_t port);
int SendUdpData(char* data,int len);
int DeInitUdp(void);

uint32_t GetIP_U32(int IP0,int IP1,int IP2,int IP3);

#endif // UDP_H_INCLUDED
