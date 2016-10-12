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
	char list_services[50] = { 0x04, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x14, 0x00, 0x01, 0x00, 0x20, 0x00, 0x43, 0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x00, 0x00 };
	char list_interfaces[26] = { 0x64, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	char path_dest_unknown[46] = { 0x6f, 0x00, 0x16, 0x00, 0x00, 0x0b, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x04, 0x00, 0x00, 0x88, 0xce, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x06, 0x00, 0x8e, 0x00, 0x05, 0x01, 0x00, 0x00 };
	char devname[68] = { 0x6f, 0x00, 0x2c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xf6, 0x95, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x1c, 0x00, 0x81, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0c, 0x00, 0xe9, 0xfd, 0x01, 0x02, 0x00, 0x00, 0x15, 0xcd, 0x5b, 0x07, 0x09, 0x4f, 0x70, 0x45, 0x4e, 0x65, 0x72, 0x72, 0x72, 0x43 };

	//loop
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	size_t packetHeadBufLen = 0;
	char packetHeadBuf[24];
	unsigned char packetHead[24];
	long describedDataLen = 0;
	long packetDataBufLen = 0;
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

				//describedDataLen = packetHead[2];

				char receivedByte[7];
				char *p;
				sprintf(receivedByte, "0x%X%X", packetHead[3], packetHead[2]);
				describedDataLen = strtol(receivedByte, &p, 16);

				printf("\r\nDescribed Data Length: ");
				printf("%u", describedDataLen);
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
						unsigned int packetDataBufPtr = 0;
						unsigned int packetDataBufRem;
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
					unsigned int i = 0;
					printf("\r\nData: ");
					for (i = 0; i < describedDataLen; i++) {
						printf("%X", packetData[i]);
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
				send(sClient, list_services, 50, 0);
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
				send(sClient, list_interfaces, 26, 0);
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
				send(sClient, path_dest_unknown, 46, 0);
				send(sClient, devname, 68, 0);

				// only have this because currently only set IP
				//closesocket(sClient);
				memset(&packetHead[0], 0, 24);
				memset(&packetData[0], 0, 65511);
				//return 0;
				//ExitThread(0);
				//interruptLoop = true;
				
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