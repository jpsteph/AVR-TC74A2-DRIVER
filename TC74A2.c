/*
 * TC74A2.c
 *
 * Created: 2/10/2023 10:50:07 PM
 *  Author: jpsteph
 */ 

#include "I2C.h"
#include "clock.h"
#include <util/delay.h>

void wakeUp(void)
{
	TWIStart();
	
	TWIWriteACK((0b10010100) | 0);
	TWIWriteACK(0x01);
	TWIWriteACK(0x00);
	
	TWIStop();
}

void sleep(void)
{
	TWIStart();
	
	TWIWriteACK((0b10010100) | 0);
	TWIWriteACK(0x01);
	TWIWriteACK(0x80);
	
	TWIStop();
	
}

//getting TC74 temp in C over i2c
uint8_t getTemp(void)
{
	uint8_t temp;
	TWIStart();
	TWIWriteACK((0b10010100) | 0);
	TWIWrite(0x00);
	
	TWIStart();
	TWIWriteACK((0b10010100) | 1);
	temp = TWIReadNACK();
	TWIStop();
	return temp;
}

float getTempMult(uint8_t count)
{
	float tempAve = 0;
	if (count == 0) count = 1;
	for(int8_t i = 0; i < count; i++)
	{
		tempAve += (float) getTemp();
		_delay_ms(30);
	}
	return tempAve / count;
}
