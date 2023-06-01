#include <stdio.h>

/* 
	原理：
		联合体 union的存放顺序是所有成员都从低地址
		开始存放，而且所有成员共享存储空间

    结果：little-endian
*/
void Endianness(void)
{
	union temp
	{
		short int a;
		char b;
	}temp;
    
	temp.a = 0x1234;
	if( temp.b == 0x12 )//低字节存的是数据的高字节数据
	{
		printf("big-endian\n");//是大端模式
	}
	else
	{
		printf("little-endian\n");//是小端模式
	}
}

int main()
{
    Endianness();
    return 0;
}