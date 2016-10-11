// SYNTP.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <time.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib" )
#define BUFFSIZE 1024


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

//======================================================================================================================
int WINAPI TestAdd(int a, int b)
{
	return a+b;	
}
//======================================================================================================================
time_t WINAPI GetTimeWithHost(char *pszHost, char *pszPort)
{
	WSADATA wsaData;
	SOCKET sockfd;
	char msg[48];
	unsigned long buffer[BUFFSIZE];
	int rv;
	int counter = 0;
	int numbytes;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;

	for(counter = 0; counter < 48; counter++)
		msg[counter] = 0;

	msg[0] = 11; //1 byte = 3 first flas set in binary: 00 001 011
	msg[1] = 0;
	msg[2] = 6;
	msg[3] = 1;
	msg[12] = 76;
	msg[13] = 79;
	msg[14] = 67;
	msg[15] = 76;


	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) 
	{		
		OutputDebugString(L"WSAStartup failed.\n");		
		return time(NULL);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM; // Use UDP

	if ((rv = getaddrinfo(pszHost, pszPort, &hints, &servinfo)) != 0) {
		//fprintf(stderr, "Getaddrinfo: %s\n", gai_strerror(rv));
		OutputDebugString(L"Getaddrinfo failed.\n");		
		return time(NULL);
	}
	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			OutputDebugString(L"Failed to create the socket\n");	
			continue;
		}		
		break;
	}
	
	//zenden en vragen naar de tijd
	if ((numbytes = sendto(sockfd, msg, (int)sizeof(msg), 0,p->ai_addr, (int)p->ai_addrlen)) == -1) {
		OutputDebugString(L"Couldn't send the message.\n");
		return time(NULL);
	}
	//printf("send len = %d\n", numbytes);

	addr_size = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, (char *)buffer, 12 * sizeof(buffer[0]) , 0,(struct sockaddr *)&their_addr, &addr_size))== -1) 
	{
		OutputDebugString(L"Failed Receive.\n");
		return time(NULL);
	}


	closesocket(sockfd); //close the socket
	WSACleanup();

	//printf("recv len = %d\n", numbytes);
	//printf("data = %s", buffer);

	int tmit=ntohl((u_long)buffer[10]);    //# get transmit time
	tmit-= 2208988800U;
	 
	//printf("tmit=%d\n",tmit);
	//time_t nowTime = (time_t)tmit;
	//printf("Time: %s",ctime(&nowTime));
	//system("PAUSE");
	return (time_t)tmit; 
}