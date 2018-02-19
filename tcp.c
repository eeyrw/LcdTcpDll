#include <winsock2.h>
#include <stdio.h>
#include "debug.h"
#include "tcp.h"


SOCKET sclient;
SOCKADDR_IN serAddr;


int TcpInit(uint32_t ipAddr,uint16_t portNum)
{
    	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data;
	if(WSAStartup(sockVersion, &data) != 0)
	{
		return -1;
	}

	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sclient == INVALID_SOCKET)
	{
		d1printf("invalid socket !");
		return -1;
	}

    serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(portNum);
	serAddr.sin_addr.S_un.S_addr = ipAddr;
	if (connect(sclient, (SOCKADDR *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		d1printf("connect error !");
		closesocket(sclient);
		return -1;
	}
		DWORD value = 1;
setsockopt( sclient, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof( value ) );

	return 1;

}

int TcpSendData(char* data,int length)
{
    int ret=send(sclient, data, length, 0);
    if(ret<0)
    {
        TcpDeInit();
        d1printf("TCP Sending Error:%d\n",ret);
    }
    return ret;
}
int TcpRecvData(char* data,int maxLength)
{
    int ret = recv(sclient, data, maxLength, 0);
    return ret;
}

int TcpDeInit(void)
{
	closesocket(sclient);
	WSACleanup();
	return 0;
}

 uint32_t GetIP_U32(int IP0,int IP1,int IP2,int IP3)
{
    uint32_t u32_IP;
    char* pu8_Addr=(char*)&u32_IP;

    pu8_Addr[0]=IP0;
    pu8_Addr[1]=IP1;
    pu8_Addr[2]=IP2;
    pu8_Addr[3]=IP3;

    return u32_IP;

}
