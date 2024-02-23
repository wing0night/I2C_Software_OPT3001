#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"
#include "OPT3001.h"
#include "stdlib.h"
#include "math.h"

int main(void)
{
	OLED_Init();
	
	OPT3001_Init();
	
	OPT3001_WriteReg(0x01, 0xC610);//���øĳ�CE10��Ӧ�����������������С
	
	uint16_t MAN_ID = OPT3001_ReadReg(0x7E);
	
	
	
	while (1)
	{
		uint16_t pow, index, remain, Result_lx;
		uint16_t Result = OPT3001_ReadReg(0x00);
		//uint16_t Result = 0x2356;//For test
		
		//����De_Num�����ɹ�
		//uint16_t test = 0x0111;
		//uint32_t test_2 = De_Num(test);
		
		//����mypow�����ɹ�
		//uint16_t test = 0x0011;	
		//uint32_t test_2;
		//test_2 = mypow(test, 2);
	
		OLED_ShowString(1, 1, "ManufacturerID:");
	
		OLED_ShowHexNum(2, 1, MAN_ID, 4);
	
		OLED_ShowString(3, 1, "Result(lx)");
		
		//����
		index = Result >> 12;
		pow = mypow(2, index);
		remain = Result & 0x0FFF;
		Result_lx = pow * De_Num(remain);//�˴��õ���lx��û��0.01����˽����ʵ���������λ��С��
		uint32_t integer = Result_lx / 100;
		uint32_t fractional = Result_lx - 100 * integer;
		
		/*pow = two_pow(Result >> 12);
		hexnum = Result & 0x0FFF;
		remain_de = De_Num(hexnum);
		uint32_t Optical_Strength = 0.01 * pow * remain_de;*/
	
		//OLED_ShowHexNum(4, 1, Result, 4);
		OLED_ShowNum(4, 1, integer, 6);
		OLED_ShowChar(4, 7, '.');
		OLED_ShowNum(4, 8, fractional, 2);
		
		Delay_ms(10);
	}
}
