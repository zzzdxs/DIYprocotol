#include <stdio.h>  
#include <winsock2.h> 
#include <string.h> 
#pragma comment(lib,"ws2_32.lib")
#define PORT 15001
#define ERROR 0  
#define BUFFER_SIZE 1024
#define DIY_HEAD 0x32
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
    return crc;
}

int compare_crc(char RecvCRC,char msgCRC)
{
	int i;
	unsigned char crcbuf;
	unsigned char msgbuf; 
	crcbuf=RecvCRC&0xFF;
	//printf("crcbuf=%x\n",crcbuf);
	msgbuf=msgCRC&0xFF;
	//printf("msgbuf=%x\n",msgbuf);
	if(crcbuf==msgbuf)
	    i=0;
    else
	    i=-1;
	return i;
}

int main()
{
	WSADATA WSAData;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in ServerAddr;   
	ServerAddr.sin_family = AF_INET;         
	ServerAddr.sin_port = htons(PORT);      
	ServerAddr.sin_addr.s_addr = INADDR_ANY; 

	bind(sock, (LPSOCKADDR)&ServerAddr, sizeof(ServerAddr));
	listen(sock, 10);
	printf("等待连接客户端...");
	SOCKET msgsock;      
	msgsock = accept(sock, (LPSOCKADDR)0, (int *)0);
	if(msgsock)
	{
		printf("%d\n",msgsock); 
		printf("连接到新的客户端...\n"); 
	}
	char buf[2048] = { 0 };//大小应该大于1024！！！
	printf("----------------SERVER----------------\n");
	while(1)
	{
		printf("--------NEW CLIENT INFO RECEIVE--------\n");
		int RecvLen=recv(msgsock, buf, sizeof(buf), 0);
		char mmsgcrc;
		//printf("%x\n",buf);
		message *msg = (message*)buf;//*******************************msg->CRC
		//sscanf(buf,"%*s,%*s,%*s,%*s,%s\n",mmsgcrc);
		//printf("mmsgcrc=%s",mmsgcrc);//sscanf读取CRC校验码
		if(msg->header.HEAD!=DIY_HEAD)
		{
			printf("receive header error\n");
			break;
		}
		int datalen=strlen(msg->buffer);
		//printf("buffer=%s\n",msg->buffer);
		//printf("crc=%s\n",buf[datalen+3]);//根据推测接收数组偏移量读取校验码
		//printf("msg->CRC=%02x\n",msg->CRC);//接收到的CRC校验码 总是不变
		char RecvCRC=crc8_maxim(msg->buffer,msg->header.DATALen);
		//printf("RecvCRC=%02x\n",RecvCRC);//根据buffer求得CRC校验码√
		int j=compare_crc(RecvCRC,msg->CRC);
		if(j==-1)
		{
			printf("receive CRC error\n");
			break;
		}
		printf("receive success\n");
		if(RecvLen!=-1)
		{
			printf("msg->header.clientaddr=%s \n", msg->header.clientaddr);
			printf("msg->buffer=%s \n", msg->buffer);
		}
		else
			break;
		
		printf("\n");
	}
    

	closesocket(sock);
	WSACleanup();

	system("pause");
	return 0;
}

