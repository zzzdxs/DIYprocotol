#include "./protocol.h"
#include <stdio.h>

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