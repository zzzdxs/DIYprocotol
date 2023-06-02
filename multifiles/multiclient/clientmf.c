#include <winsock2.h>  
#include <stdio.h> 
#include <string.h> 
#pragma comment(lib,"ws2_32.lib")
#pragma pack(4) 
#include "protocol.h"

message msg;

int main()
{
	WSADATA WSAData;
	SOCKET sock;

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in ClientAddr;
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(15001);
	ClientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sock, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr));
	printf("----------------CLIENT----------------\n");
	while(1)
	{
		printf("--------NEW CLIENT INFO SENDING--------\n");
		char snd_buf[2048] = { 0 };
		//strcpy(msg.client_name, "lyshark");
		//strcpy(msg.buffer, "bbbbbb");
		printf("msg.client_name=");
		gets(msg.header.clientaddr);
		printf("msg.buffer=");
		gets(msg.buffer);
		printf("\n");
		msg.header.DATALen=strlen(msg.buffer);
		msg.header.HEAD=DIY_HEAD;
		msg.CRC=crc8_maxim(msg.buffer,msg.header.DATALen);
		//printf("success\n");
		print_msg(msg);
		memcpy(snd_buf, &msg, sizeof(message));
		send(sock, snd_buf, sizeof(snd_buf), 0);
		//printf("%x %d\n",snd_buf,strlen(snd_buf));
		
		//printf("结构体打包前存储地址 buffer: %p  CRC: %p\n",&msg.buffer,&msg.CRC);
	}

	closesocket(sock); 
	WSACleanup();  
	return 0;
}

