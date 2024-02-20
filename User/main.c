#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"
#include "OPT3001.h"
#include "stdlib.h"

int main(void)
{
	OLED_Init();
	
	OPT3001_Init();
	
	OPT3001_WriteReg(0x01, 0xC610);//配置改成CE10响应变慢，不过噪声会变小
	
	uint16_t MAN_ID = OPT3001_ReadReg(0x7E);
	
	
	
	while (1)
	{
		uint16_t pow, remain_de, hexnum;
		uint16_t Result = OPT3001_ReadReg(0x00);
	
		OLED_ShowString(1, 1, "ManufacturerID:");
	
		OLED_ShowHexNum(2, 1, MAN_ID, 4);
	
		OLED_ShowString(3, 1, "Result(lx)");
		
		pow = two_pow(Result >> 12);
		hexnum = Result & 0x0FFF;
		remain_de = De_Num(hexnum);
		uint32_t Optical_Strength = 0.01 * pow * remain_de;
	
		//OLED_ShowHexNum(4, 1, Result, 4);
		OLED_ShowHexNum(4, 1, Result, 4);
		
		
		Delay_ms(10);
	}
}
