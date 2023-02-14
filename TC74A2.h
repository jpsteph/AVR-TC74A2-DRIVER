/*
 * TC74A2.h
 *
 * Created: 2/10/2023 10:50:28 PM
 *  Author: jpsteph
 */ 


#ifndef TC74A2_H_
#define TC74A2_H_

void wakeUp(void);

void sleep(void);

uint8_t getTemp(void);

float getTempMult(uint8_t count);

#endif /* TC74A2_H_ */