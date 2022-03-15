#ifndef DELAY_DOT_H    /* This is an "include guard" */
#define DELAY_DOT_H

// Causes a delay for n amount of cpu cycles
extern void SleepNPulses(unsigned long cycles);

// Causes a delay for n amount of microseconds
extern void SleepMicroSeconds(unsigned long microseconds);

// Causes a delay for n amount of milliseconds
extern void SleepMilliSeconds(unsigned long milliseconds);

#endif /* DELAY_DOT_H */