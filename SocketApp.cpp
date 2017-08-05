
#include "SocketApp.h"





TCP::cSocket      mi_Socket;
HANDLE            mh_Thread;




TCP::cSocket::cHash<DWORD, DWORD> mi_LocalIPs;
TCP::cSocket::cHash<SOCKET,DWORD> mi_SocketList(64);



// Get a human readable error message for an API error code
string GetErrMsg(DWORD u32_Error)
{
    // Some translations of error codes are really stupid --> show the original error code.
    string s_Code;
    switch (u32_Error)
    {
    case WSAEINTR:
        s_Code = "WSAEINTR";
        break;
    case WSAEBADF:
        s_Code = "WSAEBADF";
        break;
    case WSAEACCES:
        s_Code = "WSAEACCES";
        break;
    case WSAEFAULT:
        s_Code = "WSAEFAULT";
        break;
    case WSAEINVAL:
        s_Code = "WSAEINVAL";
        break;
    case WSAEMFILE:
        s_Code = "WSAEMFILE";
        break;
    case WSAEWOULDBLOCK:
        s_Code = "WSAEWOULDBLOCK";
        break;
    case WSAEINPROGRESS:
        s_Code = "WSAEINPROGRESS";
        break;
    case WSAEALREADY:
        s_Code = "WSAEALREADY";
        break;
    case WSAENOTSOCK:
        s_Code = "WSAENOTSOCK";
        break;
    case WSAEDESTADDRREQ:
        s_Code = "WSAEDESTADDRREQ";
        break;
    case WSAEMSGSIZE:
        s_Code = "WSAEMSGSIZE";
        break;
    case WSAEPROTOTYPE:
        s_Code = "WSAEPROTOTYPE";
        break;
    case WSAENOPROTOOPT:
        s_Code = "WSAENOPROTOOPT";
        break;
    case WSAEPROTONOSUPPORT:
        s_Code = "WSAEPROTONOSUPPORT";
        break;
    case WSAESOCKTNOSUPPORT:
        s_Code = "WSAESOCKTNOSUPPORT";
        break;
    case WSAEOPNOTSUPP:
        s_Code = "WSAEOPNOTSUPP";
        break;
    case WSAEPFNOSUPPORT:
        s_Code = "WSAEPFNOSUPPORT";
        break;
    case WSAEAFNOSUPPORT:
        s_Code = "WSAEAFNOSUPPORT";
        break;
    case WSAEADDRINUSE:
        s_Code = "WSAEADDRINUSE";
        break;
    case WSAEADDRNOTAVAIL:
        s_Code = "WSAEADDRNOTAVAIL";
        break;
    case WSAENETDOWN:
        s_Code = "WSAENETDOWN";
        break;
    case WSAENETUNREACH:
        s_Code = "WSAENETUNREACH";
        break;
    case WSAENETRESET:
        s_Code = "WSAENETRESET";
        break;
    case WSAECONNABORTED:
        s_Code = "WSAECONNABORTED";
        break;
    case WSAECONNRESET:
        s_Code = "WSAECONNRESET";
        break;
    case WSAENOBUFS:
        s_Code = "WSAENOBUFS";
        break;
    case WSAEISCONN:
        s_Code = "WSAEISCONN";
        break;
    case WSAENOTCONN:
        s_Code = "WSAENOTCONN";
        break;
    case WSAESHUTDOWN:
        s_Code = "WSAESHUTDOWN";
        break;
    case WSAETOOMANYREFS:
        s_Code = "WSAETOOMANYREFS";
        break;
    case WSAETIMEDOUT:
        s_Code = "WSAETIMEDOUT";
        break;
    case WSAECONNREFUSED:
        s_Code = "WSAECONNREFUSED";
        break;
    case WSAELOOP:
        s_Code = "WSAELOOP";
        break;
    case WSAENAMETOOLONG:
        s_Code = "WSAENAMETOOLONG";
        break;
    case WSAEHOSTDOWN:
        s_Code = "WSAEHOSTDOWN";
        break;
    case WSAEHOSTUNREACH:
        s_Code = "WSAEHOSTUNREACH";
        break;
    case WSAENOTEMPTY:
        s_Code = "WSAENOTEMPTY";
        break;
    case WSAEPROCLIM:
        s_Code = "WSAEPROCLIM";
        break;
    case WSAEUSERS:
        s_Code = "WSAEUSERS";
        break;
    case WSAEDQUOT:
        s_Code = "WSAEDQUOT";
        break;
    case WSAESTALE:
        s_Code = "WSAESTALE";
        break;
    case WSAEREMOTE:
        s_Code = "WSAEREMOTE";
        break;
    case WSASYSNOTREADY:
        s_Code = "WSASYSNOTREADY";
        break;
    case WSAVERNOTSUPPORTED:
        s_Code = "WSAVERNOTSUPPORTED";
        break;
    case WSANOTINITIALISED:
        s_Code = "WSANOTINITIALISED";
        break;
    case WSAEDISCON:
        s_Code = "WSAEDISCON";
        break;
    case WSAENOMORE:
        s_Code = "WSAENOMORE";
        break;
    case WSAECANCELLED:
        s_Code = "WSAECANCELLED";
        break;
    case WSAEINVALIDPROCTABLE:
        s_Code = "WSAEINVALIDPROCTABLE";
        break;
    case WSAEINVALIDPROVIDER:
        s_Code = "WSAEINVALIDPROVIDER";
        break;
    case WSAEPROVIDERFAILEDINIT:
        s_Code = "WSAEPROVIDERFAILEDINIT";
        break;
    case WSASYSCALLFAILURE:
        s_Code = "WSASYSCALLFAILURE";
        break;
    case WSASERVICE_NOT_FOUND:
        s_Code = "WSASERVICE_NOT_FOUND";
        break;
    case WSATYPE_NOT_FOUND:
        s_Code = "WSATYPE_NOT_FOUND";
        break;
    case WSA_E_NO_MORE:
        s_Code = "WSA_E_NO_MORE";
        break;
    case WSA_E_CANCELLED:
        s_Code = "WSA_E_CANCELLED";
        break;
    case WSAEREFUSED:
        s_Code = "WSAEREFUSED";
        break;
    case WSAHOST_NOT_FOUND:
        s_Code = "WSAHOST_NOT_FOUND";
        break;
    case WSATRY_AGAIN:
        s_Code = "WSATRY_AGAIN";
        break;
    case WSANO_RECOVERY:
        s_Code = "WSANO_RECOVERY";
        break;
    case WSANO_DATA:
        s_Code = "WSANO_DATA";
        break;
    case WSA_IO_PENDING:
        s_Code = "WSA_IO_PENDING";
        break;
    case WSA_IO_INCOMPLETE:
        s_Code = "WSA_IO_INCOMPLETE";
        break;
    case WSA_INVALID_HANDLE:
        s_Code = "WSA_INVALID_HANDLE";
        break;
    case WSA_INVALID_PARAMETER:
        s_Code = "WSA_INVALID_PARAMETER";
        break;
    case WSA_NOT_ENOUGH_MEMORY:
        s_Code = "WSA_NOT_ENOUGH_MEMORY";
        break;
    case WSA_OPERATION_ABORTED:
        s_Code = "WSA_OPERATION_ABORTED";
        break;

    default:
        s_Code=format("Code %u",u32_Error);
        break;
    }

    string s_Out;
    const DWORD BUFLEN = 1000;
    TCHAR t_Buf[BUFLEN];

    if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, u32_Error, 0, t_Buf, BUFLEN, 0))
        s_Out=format("%s:%s",s_Code.c_str(),t_Buf);
    else
        s_Out=format("%s: Windows has no explanation for this error",s_Code.c_str());
    return s_Out;
}

string format(const string& fmt, ...)
{
  va_list argptr;
  va_start(argptr, fmt);
  int bufsize = _vsnprintf(NULL, 0, fmt.c_str(), argptr) + 2;
  char* buf = new char[bufsize];
  _vsnprintf(buf, bufsize, fmt.c_str(), argptr);
  string s(buf);
  delete[] buf;
  va_end(argptr);
  return s;
}
// Formats an IP address "192.168.1.100"
string FormatIP(DWORD u32_IP)
{
    BYTE* pu8_Addr = (BYTE*)&u32_IP;
    return format("%d.%d.%d.%d", pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);
}

 DWORD GetIP_U32(int IP0,int IP1,int IP2,int IP3)
{
    DWORD u32_IP;
    BYTE* pu8_Addr=(BYTE*)&u32_IP;

    pu8_Addr[0]=IP0;
    pu8_Addr[1]=IP1;
    pu8_Addr[2]=IP2;
    pu8_Addr[3]=IP3;

    return u32_IP;

}

// Close all open sockets (if any)
void CloseSockets()
{
    if (mi_Socket.GetSocketCount())
    {
        mi_Socket.Close();
        d1printf("Socket(s) closed.");
    }
}

bool Connect(DWORD TargetIP,DWORD TargetPort)
{

//    int FreeSocketCount=mi_Socket.GetSocketCount();
//    d1printf("Free Socket num:%d",FreeSocketCount);
//    if (FreeSocketCount<=0)
//    {
//        d1printf("Socket already in use!");
//        return false;
//    }


    DWORD u32_EventTimeout = INFINITE;
    DWORD u32_Err = mi_Socket.ConnectTo(TargetIP, TargetPort, u32_EventTimeout, MAX_CLIENT_IDLE_TIME);
    if (u32_Err)
        d1printf("Connect Error %s\n", GetErrMsg(u32_Err).c_str());
    else
    {
        d1printf("Connecting to Server (%s) on Port %d.... (waiting for FD_CONNECT)", FormatIP(TargetIP).c_str(), TargetPort);

        if (MAX_CLIENT_IDLE_TIME > 0)
            d1printf("Maximum idle time: %d seconds", MAX_CLIENT_IDLE_TIME);
    }

    if (u32_Err)
    {
        CloseSockets();
        return false;
    }

    DWORD u32_ID;
    mh_Thread = CreateThread(0, 0, ProcessEventThread, 0, 0, &u32_ID);

    return true;
}


// static
ULONG WINAPI ProcessEventThread(void* p_Param)
{
    TCP_Client_ProcessEvents();
    CloseHandle(mh_Thread);
    return 0;
}

// Process all events which occur on one of the open sockets
void TCP_Client_ProcessEvents()
{

    while (TRUE) // Loop runs until the main window was closed or a severe error occurred
    {

        TCP::cSocket::cMemory* pi_RecvMem;
        SOCKET  h_Socket;
        DWORD u32_Event, u32_IP, u32_Read, u32_Sent;
        DWORD u32_Err = mi_Socket.ProcessEvents(&u32_Event, &u32_IP, &h_Socket, &pi_RecvMem, &u32_Read,  &u32_Sent);

        // Main Dialog was closed -> !Immediately! stop all output and printing into GUI.
        // Otherwise the application will not shut down correctly and the EXE keeps running. (only visible in Task Manager)
        // There may appear a lot of other strange things when the Events thread still runs while the GUI thread already finished!
//		if (mb_DlgClosed)
//			return;  // return NOT break!

        if (u32_Err == ERROR_TIMEOUT) // 50 ms interval has elapsed
            continue;

        char s_Msg[100];
        string s_Events;
        if (u32_Event) // ATTENTION: u32_Event may be == 0 -> do nothing.
        {
            d1printf("Server (%s) --> ", FormatIP(u32_IP).c_str());

            char s8_Events[200];
            mi_Socket.FormatEvents(u32_Event, s8_Events);
            s_Events += s8_Events;

            if (u32_Event & FD_READ)  d1printf("%d Bytes received.", u32_Read);
            if (u32_Event & FD_WRITE) d1printf("%d Bytes sent",      u32_Sent);



            if (u32_Event & FD_READ && pi_RecvMem) // pi_RecvMem may be NULL if an error occurred!!
            {
                ProcessReceivedDataNormal(pi_RecvMem);
            }
        }

        if (u32_Err)
        {
            // mi_Socket.Close() has been called -> don't print this error message
            if (u32_Err == WSAENOTCONN)
                break;

            // Print all the other error messages
            d1printf("ProcessEvent Error %s", GetErrMsg(u32_Err).c_str());

            // An error normally means that the socket has a problem -> abort the loop.
            // A few errors should not abort the processing:
            if (u32_Err != WSAECONNABORTED && // e.g. after the other side was killed in TaskManager
                    u32_Err != WSAECONNRESET   && // Connection reset by peer.
                    u32_Err != WSAECONNREFUSED && // FD_ACCEPT with already 62 clients connected
                    u32_Err != WSAESHUTDOWN)      // Sending data to a socket just in the short timespan
                break;                        //   between shutdown() and closesocket()
        }
    }; // end loop

    CloseSockets();
    d1printf("Connection abandoned.\r\n");
}

// ##################################################################################################
//                                  PROCESS RECEIVED DATA
// ##################################################################################################

// Mode NORMAL:
// This simple "data processor" prints the data blocks immediately and unchanged as they arrive from the network
void ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem)
{
    char*  s8_Buf = pi_RecvMem->GetBuffer();
    DWORD u32_Len = pi_RecvMem->GetLength();

    d1printf("Recv:%d bytes\n%s\n",u32_Len,s8_Buf);

    // Delete all received data from the receive memory
    pi_RecvMem->DeleteLeft(u32_Len);
}

void GetLocalIP(void)
{
    // Get all local IP addresses for each network adapter
    DWORD u32_Err = mi_Socket.GetLocalIPs(&mi_LocalIPs);
    if (u32_Err)
        d1printf("Error retrieving Local IP: %s", GetErrMsg(u32_Err).c_str());
    else
        d1printf("Successfuly retrieving Local IP!");


    if (mi_LocalIPs.GetCount() > 1)
        d1printf("More than one local IP:");

    for (DWORD i=0; i<mi_LocalIPs.GetCount(); i++)
    {
        string s_IP = FormatIP(mi_LocalIPs.GetValueByIndex(i));
        d1printf("%s",s_IP.c_str());
    }

    //return TRUE;
}


// Sends data to the given socket
// A "\r\n" in the input string is replaced with a linebreak
// returns FALSE when the sockets have been closed due to a severe error
BOOL SendToServer(uint8_t* Buf,uint32_t u32_Len)
{

    mi_Socket.GetAllConnectedSockets(&mi_SocketList);
    SOCKET h_Socket=mi_SocketList.GetKeyByIndex(0);

	d1printf("Sending %d Bytes to %s", u32_Len, FormatDisplayName(h_Socket).c_str());
	DWORD u32_Err = mi_Socket.SendTo(h_Socket, (char *)Buf, u32_Len);

	switch (u32_Err)
	{
	case 0:
		return TRUE;

	case WSAEWOULDBLOCK:
		d1printf("WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event.\n");
		return TRUE;

	case WSA_IO_PENDING:
		d1printf("WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent.\n");
		CloseSockets();
		return FALSE;

	default:
		d1printf("%s -> Error ",GetErrMsg(u32_Err).c_str());
		// Severe error -> abort event loop
		CloseSockets();
		return FALSE;
	};
}

// Format the display string for the given socket
// returns "Server (192.168.1.100)" or "Client 71C (192.168.1.100)"
string FormatDisplayName(SOCKET h_Socket)
{
	string s_IP = FormatIP(mi_SocketList.GetValueByKey(h_Socket));
	string s_Disp;
	if (mi_Socket.GetState() & TCP::cSocket::E_Server)
		s_Disp=format("Client %X (%s)", h_Socket, s_IP.c_str());
	else
        s_Disp=format("Server (%s)",s_IP.c_str());

	return s_Disp;
}


