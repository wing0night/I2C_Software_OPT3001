#include "stm32f10x.h"
#include "MyI2C.h"


//������װָ����ַ����ָ����ַд
void OPT3001_WriteReg(uint8_t RegAddress, uint16_t Data){//opt3001��ADDRESS��8λ����data��16λ
	
	uint8_t Data_Low;
	Data_Low = 0x00FF & Data;
	
	MyI2C_Start();//��������
	MyI2C_SendByte(0x88);//�ӻ���ַ
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);//ָ���Ĵ�����ַ������OPT3001�ĵ�ǰ��ַָ�룩
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data >> 8);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data_Low);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

//��������ָ����ַ��
uint16_t OPT3001_ReadReg(uint8_t RegAddress){
	uint16_t Data;//����һ���������ڴ�Ž��յ�������
	
	//��һ������һ��ָ���ӻ���ַ��ֱ�Ӹ�����һ��������ǰ�벿��
	MyI2C_Start();//��������
	MyI2C_SendByte(0x88);//�ӻ���ַ
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);//ָ���Ĵ�����ַ������OPT3001�ĵ�ǰ��ַָ�룩
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(0x89);//��дλ��Ϊ1
	MyI2C_ReceiveAck();//����Ӧ������߿���Ȩ��ʽ�����ӻ�
	Data = MyI2C_Receive2Byte();
	MyI2C_SendAck(1);//������1���������ӻ�Ӧ�𣬱�ʾ�������ݽ���
	MyI2C_Stop();
	
	return Data;
	
}

void OPT3001_Init(void){
	MyI2C_Init();//��ʼ��ʱ�ȳ�ʼ���ײ㣬��������ļ̳�
}
