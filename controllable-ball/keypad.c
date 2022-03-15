#include "keypad.h"

#ifndef GPIO_D_BASE

#define GPIO_D_BASE 0x40020C00
#define GPIO_D_MODER ((volatile unsigned long *) GPIO_D_BASE)
#define GPIO_D_OTYPER ((volatile unsigned short *) GPIO_D_BASE + 0x4)
#define GPIO_D_PUPDR ((volatile unsigned long *) GPIO_D_BASE + 0xC)
#define GPIO_D_IDR (GPIO_D_BASE + 0x10)
#define GPIO_D_ODR (GPIO_D_BASE + 0x14)

#endif

unsigned char *GPOut = 0;
volatile unsigned char *GPIn = 0;

unsigned char KeypadGetKeycode(){
	for(unsigned char i = 1; i <= 4; i++){
		KeypadActivateRow(i);
		unsigned char value = KeypadReadCol();
		
		if(value != 0){
			unsigned char mask = 1;
			for(unsigned char j = 1; j <= 4; j++){
				if((value & mask) != 0){
					return (i << 4) + j;
				}
				mask = mask << 1;
			}
		}
	}
	
	return 0xFF;
}

void KeypadActivateRow(unsigned int rowIndex){
	if(rowIndex > 4 || rowIndex == 0){
		return;
	}
	
	unsigned char binaryIndex = 1 << 3 + rowIndex;
	*GPOut = binaryIndex;
	return;
}

unsigned char KeypadReadCol(){
	return *GPIn;
}

void KeypadInit(bool usePin15To8){
	unsigned long moderRCfg = 0x5500;
	unsigned long pupdRCfg = 0xAA00;
	unsigned short otyprRCfg = 0;
	
	if(usePin15To8){
		*GPIO_D_MODER &= 0xFFFF;
		*GPIO_D_MODER |= moderRCfg << 16;
		
		*GPIO_D_OTYPER &= 0xFF;
		*GPIO_D_OTYPER |= otyprRCfg << 8;
		
		*GPIO_D_PUPDR &= 0xFFFF;
		*GPIO_D_PUPDR |= pupdRCfg << 16;
		
		GPOut = GPIO_D_ODR + 1;
		GPIn = GPIO_D_IDR + 1;
	}
	else{
		*GPIO_D_MODER &= 0xFFFF0000;
		*GPIO_D_MODER |= moderRCfg;
		
		*GPIO_D_OTYPER &= 0xFF00;
		*GPIO_D_OTYPER |= otyprRCfg;
		
		*GPIO_D_PUPDR &= 0xFFFF0000;
		*GPIO_D_PUPDR |= pupdRCfg;
		
		GPOut = GPIO_D_ODR;
		GPIn = GPIO_D_IDR;
	}
}