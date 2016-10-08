// enip-sim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

#define SESSION_REQ 0x65
#define SET_ATTR_SINGLE 0x6F

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	//initialization WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//creat slisten
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//lock IP and port 
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(44818);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//Start listen
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//loop
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("Client socket error");
			continue;
		}

		//Get Data
		memset(&revData[0], 0, 255);
		revData[0];

		int ret;

		while (true){

			ret = recv(sClient, revData, 255, 0);
			Sleep(10);

			if (ret == 0)
			{
				closesocket(sClient);
				ExitThread(0);
			}

			if (ret > 0)
			{
				//revData[ret] = 0x00;
				//revData[0];
				int i = 0;
				printf("\r\nData: ");
				for (i = 0; i < ret; i++) {
					printf("%02x", revData[i]);
				}
				printf("\r\n");

				char enipconn[28] = { 0x65, 0x00, 0x04, 0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };

				/*switch (revData[0]) {
				case 0x65:
					printf("Type: ");
					printf("Session Request\r\n");
					send(sClient, enipconn, 28, 0);
					break;
				case 0x6F:
					printf("Type: ");
					printf("Set Attribute Single\r\n");
					closesocket(sClient);
					break;
				default:
					printf("unkown command\r\n");
					break;
				}*/

				if (revData[0] == 0x65 ) {
					printf("Type: ");
					printf("Session Request\r\n");
					send(sClient, enipconn, 28, 0);
				}
				else if (revData[0] == 0x6F ) {
					printf("Type: ");
					printf("Set Attribute Single\r\n");
					closesocket(sClient);
				}
				else {
					printf("unknown command\r\n");
				}
			}

			memset(&revData[0], 0, 255);
			revData[0];

		}

		
		
	}
	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}