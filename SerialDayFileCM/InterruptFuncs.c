#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <plib/usart.h>
#include <plib/delays.h>
#include <plib/timers.h>
#include <string.h>
#include "SerialFuncs.h"
#include "InterruptFuncs.h"
#include "TimingFuncs.h"

void InitInterrupts(void)
{
    INTCONbits.PEIE=1;				//Enable peripheral interrupt
    RCONbits.IPEN=1;                            //Enable interrupt priority
    INTCONbits.GIE=1;				//Enable global interrupt
    INTCONbits.GIEH=1;                          //Enable high priority interrupt
    INTCONbits.GIEL=1;                          //Enable low priority interrupt
    INTCON2bits.TMR0IP=1;                       //Set TMR0 as high priority
    PIR1bits.RCIF=1;                            //Enable RX interrupt
    IPR1bits.RCIP=0;                            //Set receive interrupt as low priority
}

// Calendar and clock
void interrupt high_priority TimingISR(void)    //High priority ISR
{
    // Reset initial timer value
    TMR0H = 0x03;        // preset for Timer0 MSB register
    TMR0L = 0xF9;        // preset for Timer0 LSB register
    //TMR0 Overflow ISR
    //overflow=1;
    counter++;  //Increment Over Flow Counter
    //printf("%3i,%2i\r\n",counter,seconds);

    if(counter==62)
        {
        counter=0;
        seconds=seconds++;
        if (seconds>59){
            seconds=0;
            minutes = minutes++;
            if (BTcountdown==1)
            {
                BTcount=BTcount-1;
                if (BTcount==0)
                {
                    BTcountdown=0;
                    BTpower=0;
                }
            }
            if (minutes>59){
                minutes = 0;
                hours = hours++;
                if (hours>23){
                    hours = 0;
                    day=day++;
                    NewDay=1;
                    switch(month){
                        case 1: case 3: case 5: case 7 ... 8: case 10:
                            if (day==32){
                            day=1;
                            month=month++;
                            NewMonth=1;
                            }
                            break;
                        case 4: case 6: case 9: case 11:
                            if (day==31){
                            day=1;
                            month=month++;
                            NewMonth=1;
                            }
                            break;
                        case 2:
                            if ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0)){
                                if (day==30){
                                    day=1;
                                    month=month++;
                                    NewMonth=1;
                                }
                            }
                            else {
                                if (day==29){
                                    day=1;
                                    month=month++;
                                    NewMonth=1;
                                }
                            }
                            break;
                        case 12:
                            if (day==32){
                            day=1;
                            month=1;
                            year=year++;
                            }
                            break;
                        default:
                        {
                        break;
                        }

                    }
                }
            }
        }
    }
    //Clear Flag
    TMR0IF=0;
}