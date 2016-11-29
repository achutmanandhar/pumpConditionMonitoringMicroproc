
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include <plib/delays.h>
#include <plib/timers.h>
#include <string.h>
#include "InterruptFuncs.h"
#include "SerialFuncs.h"
#include "AccelerationFuncs.h"

void interrupt low_priority SerialRxPinInterrupt()
{
    //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1)
    {
        if(i<100) //our buffer size
        {
            MessageBuffer[i] = ReadUSART(); //read the byte from rx register
            //WriteUSART(MessageBuffer[i]);
            //printf("%c",MessageBuffer[i]);
            //printf("\r\n");
            if(MessageBuffer[i] == 0x0D)    //check for return key
            {
                NewMessage=1;
                printf(MessageBuffer);
                strcpy(LastMessage,MessageBuffer);
                for(;i>0;i--)
                    MessageBuffer[i] = 0x00;//clear the array
                i=0; //for sanity
                return;
            }
        i++;
        PIR1bits.RCIF = 0; // clear rx flag
        }
        else //Buffer overflow, clear buffer and reset
        {
            NewMessage=0;
            //printf(MessageBuffer);
            //printf("\r\n");
            //ClearMessageBuffer();
            for(i=20;i>0;i--)
            {
            MessageBuffer[i] = 0x00;//clear the array
            i=0; //for sanity
            }
            return;
        }
    }
}

void InitUSART(void)
{
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input

    UART1Config = USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH ;
    baud = 35;
    OpenUSART(UART1Config,baud);
    BAUDCONbits.BRG16=1;                            //16 bit comms
}

void ClearMessageBuffer(void)
{
    memset(MessageBuffer,0x00,21);

    //for(i=20;i>0;i--)
    //{
    //MessageBuffer[i] = 0x00;//clear the array
    //i=0; //for sanity
    //}
}

void    putch(char c)                           //Called by printf()
{
    while (!TXIF);
    TXREG = c;                                  //Send to serial output
}

void SetDateTime(void)
{
    printf("Set Year: ");
    while(NewMessage==0){};                 //Wait for new message
    printf(MessageBuffer);
    printf("\r\n");
    year=atoi(LastMessage);
    NewMessage=0;
    printf("Set Month: ");
    while(NewMessage==0){};                 //Wait for new message
    printf(MessageBuffer);
    printf("\r\n");
    month=atoi(LastMessage);
    NewMessage=0;
    printf("Set Day: ");
    while(NewMessage==0){};                 //Wait for new message
    printf(MessageBuffer);
    printf("\r\n");
    day=atoi(LastMessage);
    NewMessage=0;
    printf("Set Hours: ");
    while(NewMessage==0){};                 //Wait for new message
    printf(MessageBuffer);
    printf("\r\n");
    hours=atoi(LastMessage);
    NewMessage=0;
    printf("Set Minutes: ");
    while(NewMessage==0){};                 //Wait for new message
    printf(MessageBuffer);
    printf("\r\n");
    minutes=atoi(LastMessage);
    NewMessage=0;
    //printf("Set Seconds: ");
    //while(NewMessage==0){};                 //Wait for new message
    //seconds==atoi(LastMessage);
    //NewMessage=0;
    seconds=0;
    printf("Date and Time Set!\r\n");
    printf("Date: %2i/%2i/%4i\r\n",day,month,year);
    printf("Time: %2i:%2i:%2i\r\n",hours,minutes,seconds);
    NewDay=1;
    NewMonth=1;
}

void MessageRoutine(void)
{
    //Logging=0;
    char str1[20];
    char str2[20];
    int ret;

    NewMessage=0;
    //printf("New Message!\r\n");
    strcpy(str1,LastMessage);
    strcpy(str2,ConfigWord);
    ret = strcmp(str1,str2);
    if (ret==0)
    {
        SetDateTime();
    }

    strcpy(str2,"SET\r");
    ret = strcmp(str1,str2);
    if (ret==0)
    {
        printf(MessageBuffer);
        printf("\r\n");
        printf("Set offcountmax (default 2000):\r\n");
        while(NewMessage==0){};                 //Wait for new message
        printf(MessageBuffer);
        printf("\r\n");
        offcountmax=atoi(LastMessage);
        NewMessage=0;

        printf("Set Nlong (default 50):\r\n");
        while(NewMessage==0){};                 //Wait for new message
        printf(MessageBuffer);
        printf("\r\n");
        Nlong=atoi(LastMessage);
        NewMessage=0;

        printf("Set Nshort (default 10):\r\n");
        while(NewMessage==0){};                 //Wait for new message
        printf(MessageBuffer);
        printf("\r\n");
        Nshort=atoi(LastMessage);
        NewMessage=0;

        printf("Set onthresh (default 2000):\r\n");
        while(NewMessage==0){};                 //Wait for new message
        printf(MessageBuffer);
        printf("\r\n");
        onthresh=atoi(LastMessage);
        NewMessage=0;
        printf("Setting Complete!\r\n");
    }

    strcpy(str2,"Time?\r");
    ret = strcmp(str1,str2);
    if (ret==0)
    {
        printf(MessageBuffer);
        printf("\r\n");
        printf("Time: %2i:%2i:%2i\r\n",hours,minutes,seconds);
    }
    strcpy(str2,"Date?\r");
    ret = strcmp(str1,str2);
    if (ret==0)
    {
        printf(MessageBuffer);
        printf("\r\n");
        printf("Date: %2i/%2i/%4i\r\n",day,month,year);
    }
    strcpy(str2,"LOG\r");
    ret = strcmp(str1,str2);
    if (ret==0)
    {
        printf("Switching over to Data Logging.\r\n"
               "To reconnect via bluetooth, the logger must be power cycled.\r\n"
               "Bluetooth module will be powered down now.\r\n");
        Delay1KTCYx(1000);
        BTPower=0;
    

                /*
               "If J_RX is populated, you can still see the data sent to the OpenLog.\r\n"
               "Would you like the bluetooth module to stay on? (y/n): ");
        while(NewMessage==0){};                 //Wait for new message
        NewMessage=0;
        strcpy(str1,LastMessage);
        strcpy(str2,"y\r");
        ret = strcmp(str1,str2);
        if (ret==0)
        {
            printf("\nFor how many minutes?\r\n");
            while(NewMessage==0){};                 //Wait for new message
            NewMessage=0;
            BTcount=atoi(LastMessage);
            BTcountdown=1;
            printf("OK. Bluetooth module will be powered down in %3i minutes.\r\n",BTcount);
        }
        else
        {
            printf("\nOK. Bluetooth module will be powered down now.\r\n");
            Delay1KTCYx(1000);
            BTPower=0;
        }
                 * */
    NewMessage=0;
    SerialSource=OpenLog;
    }
}