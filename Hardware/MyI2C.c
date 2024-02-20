#include "stm32f10x.h"
#include "Delay.h"

//��ʼ������
void MyI2C_Init(void){
	//���i2cֻ��Ҫ�õ�GPIO�Ķ�д����˲���Ҫ���⺯��
	
	//��������ʼ��GPIO����PB10��PB11����ΪI2cͨѶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ʼ��Ϊ��©���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//�˿ڸ�Ϊpin10��pin11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);//��ʼ�������һ��������Setbits��pin10��pin11���øߵ�ƽ��ΪͨѶ��׼����
	
}

//���������ú���ʵ��6��ʱ��Ԫ


void MyI2C_W_SCL(uint8_t BitValue){//���ͷ�or����SCL����Ϊ��װΪһ������
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//����������ǿתΪbitaction���͸�ֵ��SCL
	Delay_us(10);//��ֹ��Ƭ����Ƶ����MPU6050�����ϣ���˴˴�����һ����ʱ��������Ƭ���ͺ�ʱ����Ҫ����ʱ���Ƚ����޸�
}

void MyI2C_W_SDA(uint8_t BitValue){//��SDAҲ���з�װ
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void){//�Զ�SDAұ��ѧ��װ
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;

}
//�������϶���ĺ���������װ���к���
void MyI2C_Start(void){
	MyI2C_W_SDA(1);//������SDA��������ʼ�������ظ���ʼ������
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);//������ֹ������SCL�Ըߵ�ƽ�������������е�Ԫ����SCL�Ե͵�ƽ����
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte){
	uint8_t i;
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x80 >> i));//�˴��ð�λ��ķ�ʽ����ȡ��Byte�����λ
		MyI2C_W_SCL(1);//�ͷ�SCL��������
		MyI2C_W_SCL(0);//����SCL������������
	}
}



uint8_t MyI2C_ReceiveByte(void){
	uint8_t Byte = 0x00, i;
	MyI2C_W_SDA(1);
	for(i = 0; i<8; i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}//��SDA�ж���������д�뵽Byte
		MyI2C_W_SCL(0);
	}
	return Byte;
}



//�������Ƿ���Ӧ��ͽ���Ӧ����
void MyI2C_SendAck(uint8_t AckBit){
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void){
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
	
uint16_t MyI2C_Receive2Byte(void){
	uint16_t Byte = 0x0000, i;
	MyI2C_W_SDA(1);
	for(i = 0; i<8; i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1){Byte |= (0x8000 >> i);}//��SDA�ж���������д�뵽Byte
		MyI2C_W_SCL(0);
		
	}
	MyI2C_SendAck(0);
	
	MyI2C_W_SDA(1);
	for(i = 0; i<8; i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1){Byte |= (0x8000 >> (i+8));}//��SDA�ж���������д�뵽Byte
		MyI2C_W_SCL(0);
		
	}
	
	return Byte;
}

void MyI2C_Send2Byte(uint16_t Byte){
	uint8_t i;
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x8000 >> i));//�˴��ð�λ��ķ�ʽ����ȡ��Byte�����λ
		MyI2C_W_SCL(1);//�ͷ�SCL��������
		MyI2C_W_SCL(0);//����SCL������������
	}
	MyI2C_ReceiveAck();
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x8000 >> (i+8)));//�˴��ð�λ��ķ�ʽ����ȡ��Byte�����λ
		MyI2C_W_SCL(1);//�ͷ�SCL��������
		MyI2C_W_SCL(0);//����SCL������������
	}
}




