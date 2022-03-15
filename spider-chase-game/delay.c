#include "delay.h"

#ifndef SYS_TICK
// Variables for manual systick management
#define SYS_TICK 0xE000E010
#define STK_CTRL ((volatile unsigned long *) SYS_TICK)
#define STK_LOAD ((unsigned long *) SYS_TICK + 0x4)
#define STK_VAL ((volatile unsigned long *) SYS_TICK + 0x8)

#endif /* SYS_TICK */

void SleepNPulses(unsigned long cycles){
	*STK_CTRL = 0;
	*STK_LOAD = cycles;
	*STK_VAL = cycles;
	unsigned long mask = 0x10000;
	unsigned long temp = 0;
	*STK_CTRL = 0x5;
	
	do{
		temp = *STK_CTRL & mask;
	}while(!temp);
	
	*STK_CTRL = 0;
	return;
}

void SleepMicroSeconds(unsigned long microseconds){
	for(unsigned long i = 0; i < microseconds; i++){
		SleepNPulses(168);
	}
	return;
}

void SleepMilliSeconds(unsigned long milliseconds){
	SleepMicroSeconds(milliseconds * 10); // should be 1000 but it took a long time in simulation
	return;
}