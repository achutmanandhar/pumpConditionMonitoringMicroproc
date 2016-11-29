//******Functions Relating to Timing ******//

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <plib/timers.h>
#include "TimingFuncs.h"

void InitTimer0(void)
{
    Timer0Config = TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1 ;
    OpenTimer0(Timer0Config);
}

void InitTimer1(void)
{
    Timer1Config = T1_8BIT_RW & T1_SOURCE_INT & T1_PS_1_1 & T1_OSC1EN_OFF;
    OpenTimer1(Timer1Config);
    T1CONbits.TMR1ON=0;
    T1CONbits.T1OSCEN=0;
    PIE1bits.TMR1IE=0;
    //TRISCbits.RC0=0;
    //T1CONbits.TMR1CS=0;
    //T1CONbits.TMR1ON=0;
    //T1CONbits.T1OSCEN=0;
    //T1CON = 0x00;
    //CCP2CON = 0x00;
}

void TimeLoop(void)
{
    tr = ReadTimer0();              //Read Value of Timer1
    //printf("%u,%u,%u\r\n", tr, tl, tt);
    if (tl>tr)
    {
        tt = tr-tl-1017;                //Calculate time taken since last read
        //overflow=0;                     //Compensating for timer overflow
    }
    else
    {
        tt = tr-tl;                     //Calculate time taken since last read
    }
    tl = tr;                        //Set 'last time' as most recent read
}