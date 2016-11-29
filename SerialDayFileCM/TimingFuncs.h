/* 
 * File:   TimingFuncs.h
 * Author: Ben
 *
 * Created on 12 September 2015, 14:13
 */

#ifndef TIMINGFUNCS_H
#define	TIMINGFUNCS_H

//Variables
unsigned char Timer0Config;             //Character store for timer0 configuration
unsigned char Timer1Config;             //Character store for timer1 configuration
unsigned int overflow;

unsigned int tl=0;                      //Clock cycles per program cycle
unsigned int tr=0;
unsigned int tt=0;

void InitTimer0(void);
void InitTimer1(void);
void TimeLoop(void);

#endif	/* TIMINGFUNCS_H */

