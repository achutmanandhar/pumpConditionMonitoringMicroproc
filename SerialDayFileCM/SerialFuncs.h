/* 
 * File:   SerialFuncs.h
 * Author: Ben
 *
 * Created on 12 September 2015, 15:47
 */

#ifndef SERIALFUNCS_H
#define	SERIALFUNCS_H

//#define LogPower LATAbits.LATA0         //Use THERM1 to turn logger on
#define LogPower LATCbits.LATC0         //Use ON_KEY to turn logger on
#define Indicator LATAbits.LATA1        //Use THERM2 as indicator
#define SerialSource LATBbits.LATB3     //Use DSR to select Openlog(0) or BT(1)
#define BTPower LATBbits.LATB4          //Use CTS to turn BT on
#define OpenLog 1
#define BlueTooth 0

unsigned char MsgFromPIC[] = "\r\nYou typed :";
char rx;

// Stores for received messages
unsigned char MessageBuffer[20];
unsigned char LastMessage[20];
int i=0;
unsigned char ConfigWord[20]="CONFIG\r";
unsigned int NewMessage=0;

unsigned char UART1Config = 0, baud = 0;

void interrupt low_priority SerialRxPinInterrupt();
void    putch(char);                           //Called by printf()
void InitUSART(void);
void ClearMessageBuffer(void);
void MessageRoutine(void);
void SetDateTime(void);

#endif	/* SERIALFUNCS_H */

