#include <WINSOCK2.H>
#include <STDIO.H>
#include "debug.h"
#include "tcp.h"
#include <stdint.h>


SOCKET sclient;
SOCKADDR_IN serAddr;

int ResolveHostName(char* host_name,uint32_t *IP)
{
    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;

    DWORD dwError;
    int i = 0;

    struct hostent *remoteHost;
    struct in_addr addr;

    char **pAlias;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        d1printf("WSAStartup failed: %d\n", iResult);
        return -1;
    }

    d1printf("Calling gethostbyname with %s\n", host_name);
    remoteHost = gethostbyname(host_name);

    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                d1printf("Host not found\n");
                return -1;
            } else if (dwError == WSANO_DATA) {
                d1printf("No data record found\n");
                return -1;
            } else {
                d1printf("Function failed with error: %ld\n", dwError);
                return -1;
            }
        }
    } else {
        d1printf("Function returned:\n");
        d1printf("\tOfficial name: %s\n", remoteHost->h_name);
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            d1printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
        }
        d1printf("\tAddress type: ");
        switch (remoteHost->h_addrtype) {
        case AF_INET:
            d1printf("AF_INET\n");
            break;
        case AF_NETBIOS:
            d1printf("AF_NETBIOS\n");
            break;
        default:
            d1printf(" %d\n", remoteHost->h_addrtype);
            break;
        }
        d1printf("\tAddress length: %d\n", remoteHost->h_length);

        i = 0;
        if (remoteHost->h_addrtype == AF_INET)
        {
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                d1printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
            }
            if(i>0)
                (*IP) = *(u_long *) remoteHost->h_addr_list[--i];
            else
                return -1;
        }
        else if (remoteHost->h_addrtype == AF_NETBIOS)
        {
            d1printf("NETBIOS address was returned\n");
        }
    }

    return 1;

}

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
