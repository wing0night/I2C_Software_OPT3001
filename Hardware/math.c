#include "stm32f10x.h"

uint16_t two_pow(uint16_t y){
	uint32_t Result = 1;
	while(y--){
		Result *= 2;
	}
	return Result;
}

uint32_t pow(uint16_t x, uint16_t y){
	uint32_t Result = 1;
	while(y--){
		Result *= x;
	}
	return Result;
}

uint32_t De_Num(uint16_t Hex_Num){
	uint8_t i;
	uint32_t De_Num = 0;
	for(i = 0; i< 16;i++){
		De_Num = De_Num + ((0x0001 << i) & Hex_Num) * pow(2, i);
	}
	return De_Num;
}


