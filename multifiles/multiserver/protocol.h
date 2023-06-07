#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define PORT 15001
#define ERROR 0  
#define BUFFER_SIZE 1024
#define DIY_HEAD 0x32

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

char crc8_maxim(char *data, int length);
int compare_crc(char RecvCRC,char msgCRC);
void print_msg(message msg);

#endif