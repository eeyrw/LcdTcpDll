#include "udp.h"
#include <winsock2.h>
#include "debug.h"


SOCKET sclient;

SOCKADDR_IN sc;
int len_of_sin;

 uint32_t GetIP_U32(int IP0,int IP1,int IP2,int IP3)
{
    uint32_t u32_IP;
    char* pu8_Addr=(char*)&u32_IP;

    pu8_Addr[0]=IP0;
    pu8_Addr[1]=IP1;
    pu8_Addr[2]=IP2;
    pu8_Addr[3]=IP3;
    d1printf("Target IP: %d.%d.%d.%d",pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);

    return u32_IP;

}
int UdpInit(uint32_t u32Ip,uint32_t port)
{
    WORD socketVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(socketVersion, &wsaData) != 0)
    {
        return -1;
    }
    sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sc.sin_family = AF_INET;
    sc.sin_port = htons(port);
    sc.sin_addr.S_un.S_addr = u32Ip;
    len_of_sin = sizeof(sc);

    return 1;
}


int SendUdpData(char* data,int length)
{

    int sendLen;
    sendLen=sendto(sclient, data, length, 0, (SOCKADDR *)&sc, len_of_sin);


//    char recvData[255];
//    int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sc, &len);
//    if(ret > 0)
//    {
//        recvData[ret] = 0x00;
//        printf(recvData);
d1printf("SendLen = %d\n", sendLen);
//    }
if(sendLen < 0)
    {
        int error = WSAGetLastError();
        d1printf("error = %d\n", error);
    }
    return sendLen;

}

int DeInitUdp(void)
{
    closesocket(sclient);
    WSACleanup();
}
