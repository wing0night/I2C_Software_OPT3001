#ifndef __OPT3001_H
#define __OPT3001_H

void OPT3001_WriteReg(uint8_t RegAddress, uint16_t Data);

uint16_t OPT3001_ReadReg(uint8_t RegAddress);

void OPT3001_Init(void);


#endif


