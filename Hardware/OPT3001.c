#include "stm32f10x.h"
#include "MyI2C.h"


//先来封装指定地址读和指定地址写
void OPT3001_WriteReg(uint8_t RegAddress, uint16_t Data){//opt3001的ADDRESS是8位，而data是16位
	
	uint8_t Data_Low;
	Data_Low = 0x00FF & Data;
	
	MyI2C_Start();//开启传输
	MyI2C_SendByte(0x88);//从机地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);//指定寄存器地址（存在OPT3001的当前地址指针）
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data >> 8);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data_Low);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

//接下来是指定地址读
uint16_t OPT3001_ReadReg(uint8_t RegAddress){
	uint16_t Data;//定义一个变量用于存放接收到的数据
	
	//第一步还是一样指定从机地址，直接复制上一个函数的前半部分
	MyI2C_Start();//开启传输
	MyI2C_SendByte(0x88);//从机地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);//指定寄存器地址（存在OPT3001的当前地址指针）
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(0x89);//读写位改为1
	MyI2C_ReceiveAck();//接收应答后总线控制权正式交给从机
	Data = MyI2C_Receive2Byte();
	MyI2C_SendAck(1);//参数给1，即不给从机应答，表示接收数据结束
	MyI2C_Stop();
	
	return Data;
	
}

void OPT3001_Init(void){
	MyI2C_Init();//初始化时先初始化底层，类似于类的继承
}
