#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"
#include "OPT3001.h"

int main(void)
{
	OLED_Init();
	
	OPT3001_Init();
	
	//OPT3001_WriteReg(0x01, 0x0000);
	
	uint16_t ID = OPT3001_ReadReg(0x01);
	
	OLED_ShowHexNum(1, 1, ID, 4);
	
	while (1)
	{
		
		
		//Delay_ms(50);
	}
}
