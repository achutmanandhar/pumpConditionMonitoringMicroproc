/* 
 * File:   InterruptFuncs.h
 * Author: Ben
 *
 * Created on 13 September 2015, 00:26
 */

#ifndef INTERRUPTFUNCS_H
#define	INTERRUPTFUNCS_H

//Counters
unsigned int counter=0;                 //Overflow counter
unsigned int BTcount, BTcountdown=0, BTpower;

unsigned int overflow;

// Stores for time and date
unsigned int seconds=0;
unsigned int minutes=0;
unsigned int hours=0;
unsigned int day=1;
unsigned int month=1;
unsigned int year=2015;

unsigned int NewDay=1;
unsigned int NewMonth=1;

void InitInterrupts(void);
void interrupt high_priority TimingISR(void);

#endif	/* INTERRUPTFUNCS_H */

