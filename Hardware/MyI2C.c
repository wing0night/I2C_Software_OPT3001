#include "stm32f10x.h"
#include "Delay.h"

//初始化函数
void MyI2C_Init(void){
	//软件i2c只需要用到GPIO的读写，因此不需要看库函数
	
	//接下来初始化GPIO。将PB10和PB11配置为I2c通讯口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//初始化为开漏输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//端口改为pin10和pin11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);//初始化的最后一步是利用Setbits将pin10和pin11都置高电平（为通讯做准备）
	
}

//接下来配置函数实现6个时序单元


void MyI2C_W_SCL(uint8_t BitValue){//将释放or拉低SCL的行为封装为一个函数
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//将传入数据强转为bitaction类型赋值给SCL
	Delay_us(10);//防止单片机主频过高MPU6050跟不上，因此此处加了一个延时。更换单片机型号时可能要对延时长度进行修改
}

void MyI2C_W_SDA(uint8_t BitValue){//对SDA也进行封装
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void){//对读SDA冶金学封装
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;

}
//利用以上定义的函数继续封装下列函数
void MyI2C_Start(void){
	MyI2C_W_SDA(1);//先拉低SDA（兼容起始条件和重复起始条件）
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);//除了终止条件中SCL以高电平结束，其他所有单元都是SCL以低电平结束
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte){
	uint8_t i;
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x80 >> i));//此处用按位与的方式依次取出Byte的最高位
		MyI2C_W_SCL(1);//释放SCL，读数据
		MyI2C_W_SCL(0);//拉低SCL，继续读数据
	}
}



uint8_t MyI2C_ReceiveByte(void){
	uint8_t Byte = 0x00, i;
	MyI2C_W_SDA(1);
	for(i = 0; i<8; i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}//将SDA中读到的数据写入到Byte
		MyI2C_W_SCL(0);
	}
	return Byte;
}



//接下来是发送应答和接收应答函数
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
		if(MyI2C_R_SDA() == 1){Byte |= (0x8000 >> i);}//将SDA中读到的数据写入到Byte
		MyI2C_W_SCL(0);
		
	}
	MyI2C_SendAck(0);
	
	MyI2C_W_SDA(1);
	for(i = 0; i<8; i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1){Byte |= (0x8000 >> (i+8));}//将SDA中读到的数据写入到Byte
		MyI2C_W_SCL(0);
		
	}
	
	return Byte;
}

void MyI2C_Send2Byte(uint16_t Byte){
	uint8_t i;
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x8000 >> i));//此处用按位与的方式依次取出Byte的最高位
		MyI2C_W_SCL(1);//释放SCL，读数据
		MyI2C_W_SCL(0);//拉低SCL，继续读数据
	}
	MyI2C_ReceiveAck();
	for(i = 0; i<8; i++){
		MyI2C_W_SDA(Byte & (0x8000 >> (i+8)));//此处用按位与的方式依次取出Byte的最高位
		MyI2C_W_SCL(1);//释放SCL，读数据
		MyI2C_W_SCL(0);//拉低SCL，继续读数据
	}
}




