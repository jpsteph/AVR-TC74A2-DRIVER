/*
 * Tempsensi2c.c
 *
 * Created: 2/4/2023 1:01:12 PM
 * Author : jpsteph
 */ 

#include <avr/io.h>
#include "I2C.h"
#include <string.h>
#include "clock.h"
#include <util/delay.h>
#include "uart.h"
#include "TC74A2.h"

#define BAUDRATE 9600

#define LED 7
#define LEDINIT() DDRC |= (1 << LED)
#define LEDBLINK() PORTC |= (1 << LED);\
					_delay_ms(500);\
					PORTC &= ~(1 << LED);\
					_delay_ms(500);

int main(void)
{
	
	USART_Init(BAUDRATE);
	LEDINIT();
	TWIInit(100000);  //100khz
	
    /* Replace with your application code */
	//wakeUp();
    while (1) 
    {
		wakeUp();
		_delay_ms(20);
		//USART_Transmit_Int8(getTemp());
		USART_Transmit_Float(getTempMult(50));
		sleep();
		
		LEDBLINK();
    }
}

