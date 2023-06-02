#include <winsock2.h>  
#include <stdio.h> 
#include <string.h> 
#pragma comment(lib,"ws2_32.lib")
#define Client_ADDR 'cilent'
#define DIY_HEAD 0x32
#define BUFFER_SIZE 1024
#pragma pack(4) 

typedef struct HEADER
{
	int DATALen;
	char HEAD;
	char clientaddr[30];
	
	
}Header;

typedef struct Info
{
	Header header;
	char buffer[BUFFER_SIZE];
	char CRC;
}message;

char crc8_maxim(char *data, int length)//DS18B20
{
    int i;
    char crc = 0;         // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8C;        // 0x8C = reverse 0x31
            else
                crc >>= 1;
        }
    }
    //printf("CRC=%02x\n",crc);
    return crc;
}

void print_msg(message msg)
{
	printf("msg.Header.HEAD: %x\n",msg.header.HEAD);
	printf("msg.Header.clientaddr: ");
	printf("%x\n",msg.header.clientaddr);
	printf("msg.Header.DATALen: %x\n",msg.header.DATALen);
	printf("msg.buffer: %x\n",msg.buffer);
	printf("msg.RecvCRC: %02x\n",msg.CRC);
	printf("\n");
}

message msg;


void GetSysInfo()
{
	SYSTEM_INFO sysInfo;
}

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

