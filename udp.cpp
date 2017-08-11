#include "udp.h"
#include <winsock2.h>


SOCKET sclient;

sockaddr_in sin;
int len_of_sin;

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
int UdpInit(uint32_t u32Ip,uint32_t port)
{
    WORD socketVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(socketVersion, &wsaData) != 0)
    {
        return -1;
    }
    sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = u32Ip;
    len_of_sin = sizeof(sin);

    return 1;
}


int SendUdpData(char* data,int length)
{

    return sendto(sclient, data, length, 0, (sockaddr *)&sin, len_of_sin);

//    char recvData[255];
//    int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
//    if(ret > 0)
//    {
//        recvData[ret] = 0x00;
//        printf(recvData);
//    }

}

int DeInitUdp(void)
{
    closesocket(sclient);
    WSACleanup();
}
