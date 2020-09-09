#ifndef TCP_H_INCLUDED
#define TCP_H_INCLUDED
#include <stdint.h>

extern int TcpInit(uint32_t ipAddr, uint16_t portNum);
extern int TcpSendData(char *data, int length);
extern int TcpRecvData(char *data, int maxLength);
extern int TcpDeInit(void);
extern uint32_t GetIP_U32(int IP0, int IP1, int IP2, int IP3);

#endif // TCP_H_INCLUDED
