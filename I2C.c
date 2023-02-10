/*
 * I2C.c
 *
 * Created: 1/27/2023 9:46:05 PM
 *  Author: jpsteph
 */ 


#include <avr/io.h>
#include "clock.h"
#include <util/delay.h>
#include "I2C.h"

//init i2c
void TWIInit(uint32_t speed) {
	uint32_t gen_t;
	TWSR = 0x00;
	gen_t = (((F_CPU/speed) - 16) / 2) & 0xFF;
	TWBR = gen_t & 0xFF;
	
	//set SCL to 400kHz
	//TWBR = 0x0C;
	
	//enable TWI
	TWCR = (1<<TWEN);
}

//send start signal
void TWIStart(void) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

//send stop signal
void TWIStop(void) {
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

//writes via i2c
void TWIWrite(uint8_t u8data) {
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

void TWIWriteRestartACK(uint8_t u8data) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
}

//writes via i2c
void TWIWriteACK(uint8_t u8data) {
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
}

//read byte with ACK
uint8_t TWIReadACK(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

//read byte with No ACK
uint8_t TWIReadNACK(void) {
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIGetStatus(void) {
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}


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

