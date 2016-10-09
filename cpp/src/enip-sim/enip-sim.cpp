// enip-sim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

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

	// Initialize a bunch of variables we will use

	// enip_conn
	char register_session_resp[28] = { 0x65, 0x00, 0x04, 0x00, 0xEF, 0xBE, 0xAD, 0xDE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };

	//loop
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	size_t packetHeadBufLen = 0;
	char packetHeadBuf[24];
	unsigned char packetHead[24];
	size_t describedDataLen = 0;
	size_t packetDataBufLen = 0;
	char packetDataBuf[65511];
	unsigned char packetData[65511];
	while (true)
	{
		
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("Client socket error");
			continue;
		}

		//Get Data
		memset(&packetHeadBuf[0], 0, 24);
		packetHeadBuf[0];

		int ret;

		while (true){

			ret = recv(sClient, packetHeadBuf, 24, 0);
			Sleep(10);

			if (ret == 0)
			{
				closesocket(sClient);
				ExitThread(0);
			}

			if (ret > 0 && ret <= 24)
			{
				
				if (ret == 24) {
					memcpy(packetHead, packetHeadBuf, 24);
				}
				else {
					printf("\r\nDidn't get all 24 bytes");
					printf("\r\nCaptured: ");
					printf("%u", ret);
					packetHeadBufLen = ret;
					size_t packetHeadBufPtr = 0;
					size_t packetHeadBufRem;
					packetHeadBufRem = 24 - ret;
					memcpy(packetHead + packetHeadBufPtr, packetHeadBuf, ret);
					packetHeadBufPtr = packetHeadBufPtr + ret;
					
					memset(&packetHeadBuf[0], 0, 24);
					packetHeadBuf[0];



					/* DEBUGGING
					printf("\r\nCopied: ");
					//printf("%02x", packetHead[packetHeadBufPtr]);
					int i = 0;
					for (i = 0; i < packetHeadBufPtr; i++) {
						printf("%02x", packetHead[i]);
					}
					printf("\r\nTo Pointer Location: ");
					printf("%u", packetHeadBufPtr);
					*/



					printf("\r\nLooping");
					while (packetHeadBufRem > 0) {
						
						/*DEBUGGING
						printf("\r\n\r\nRequesting: ");
						printf("%u", packetHeadBufRem);
						*/

						ret = recv(sClient, packetHeadBuf, packetHeadBufRem, 0); //DISABLE TO FORCE MORE CHECKS
						//ret = recv(sClient, packetHeadBuf, 1, 0); //ENABLE TO FORCE MORE CHECKS
						

						/*DEBUGGING
						printf("\r\nCaptured: ");
						printf("%u", ret);
						*/


						packetHeadBufRem = packetHeadBufRem - ret;



						/*DEBUGGING
						printf("\r\nRemaining: ");
						printf("%u", packetHeadBufRem);
						*/
						
						
						memcpy(packetHead + packetHeadBufPtr, packetHeadBuf, ret);
						packetHeadBufPtr = packetHeadBufPtr + ret;




						/*DEBUGGING
						printf("\r\nCopied: ");
						//printf("%02x", packetHead[packetHeadBufPtr]);
						int i = 0;
						for (i = 0; i < packetHeadBufPtr; i++) {
							printf("%02x", packetHead[i]);
						}
						*/




						memset(&packetHeadBuf[0], 0, 24);
						packetHeadBuf[0];



						/*DEBUGGING
						printf("\r\nTo Pointer Location: ");
						printf("%u", packetHeadBufPtr);

						printf("\r\nRestarting Loop");
						*/
					}
				}

				//revData[ret] = 0x00;
				//revData[0];
				int i = 0;
				printf("\r\nHeader: ");
				for (i = 0; i < 24; i++) {
					printf("%02x", packetHead[i]);
				}
				printf("\r\n");

				describedDataLen = packetHead[2];

				printf("\r\nDescribed Data Length: ");
				printf("%02x", describedDataLen);
				printf("\r\n");





				

			}

			memset(&packetHeadBuf[0], 0, 24);
			packetHeadBuf[0];

			
			
			
			
			if (describedDataLen > 0) {






				ret = recv(sClient, packetDataBuf, describedDataLen, 0);
				Sleep(10);

				/*if (ret == 0)
				{
				closesocket(sClient);
				ExitThread(0);
				}*/

				if (ret > 0 && ret <= describedDataLen)
				{

					if (ret == describedDataLen) {
						memcpy(packetData, packetDataBuf, describedDataLen);
					}
					else {
						printf("\r\nDidn't get all "); 
						printf("%u", describedDataLen);
						printf(" bytes");
						printf("\r\nCaptured: ");
						printf("%u", ret);
						packetDataBufLen = ret;
						size_t packetDataBufPtr = 0;
						size_t packetDataBufRem;
						packetDataBufRem = describedDataLen - ret;
						memcpy(packetData + packetDataBufPtr, packetDataBuf, ret);
						packetDataBufPtr = packetDataBufPtr + ret;

						memset(&packetDataBuf[0], 0, describedDataLen);
						packetDataBuf[0];



						/* DEBUGGING
						printf("\r\nCopied: ");
						//printf("%02x", packetHead[packetHeadBufPtr]);
						int i = 0;
						for (i = 0; i < packetHeadBufPtr; i++) {
						printf("%02x", packetHead[i]);
						}
						printf("\r\nTo Pointer Location: ");
						printf("%u", packetHeadBufPtr);
						*/



						printf("\r\nLooping");
						while (packetDataBufRem > 0) {

							/*DEBUGGING
							printf("\r\n\r\nRequesting: ");
							printf("%u", packetHeadBufRem);
							*/

							ret = recv(sClient, packetDataBuf, packetDataBufRem, 0); //DISABLE TO FORCE MORE CHECKS
																					 //ret = recv(sClient, packetHeadBuf, 1, 0); //ENABLE TO FORCE MORE CHECKS


																					 /*DEBUGGING
																					 printf("\r\nCaptured: ");
																					 printf("%u", ret);
																					 */


							packetDataBufRem = packetDataBufRem - ret;



							/*DEBUGGING
							printf("\r\nRemaining: ");
							printf("%u", packetHeadBufRem);
							*/


							memcpy(packetData + packetDataBufPtr, packetDataBuf, ret);
							packetDataBufPtr = packetDataBufPtr + ret;




							/*DEBUGGING
							printf("\r\nCopied: ");
							//printf("%02x", packetHead[packetHeadBufPtr]);
							int i = 0;
							for (i = 0; i < packetHeadBufPtr; i++) {
							printf("%02x", packetHead[i]);
							}
							*/




							memset(&packetDataBuf[0], 0, describedDataLen);
							packetDataBuf[0];



							/*DEBUGGING
							printf("\r\nTo Pointer Location: ");
							printf("%u", packetHeadBufPtr);

							printf("\r\nRestarting Loop");
							*/
						}
					}

					//revData[ret] = 0x00;
					//revData[0];
					int i = 0;
					printf("\r\nData: ");
					for (i = 0; i < describedDataLen; i++) {
						printf("%02x", packetData[i]);
					}
					printf("\r\n");




				}







			}





			bool interruptLoop = false;

			switch (packetHead[0]) {
			case 0x00:
				printf("Type: NOP\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x04:
				printf("Type: List Services\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x63:
				printf("Type: List Identity\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x64:
				printf("Type: List Interfaces\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x65:
				printf("Type: Register Session\r\n");
				send(sClient, register_session_resp, 28, 0);
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x66:
				printf("Type: Un-Register Session\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x6F:
				printf("Type: Set Attribute Single\r\n");
				// only have this because currently only set IP
				closesocket(sClient);
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				//return 0;
				//ExitThread(0);
				interruptLoop = true;
				
				break;
			case 0x70:
				printf("Type: Send Unit Data\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x72:
				printf("Type: Indicate Status\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			case 0x73:
				printf("Type: Cancel\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			default:
				printf("unkown command\r\n");
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				break;
			}

			if (interruptLoop == true) {
				break;
			}

		}

		
		
	}
	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}