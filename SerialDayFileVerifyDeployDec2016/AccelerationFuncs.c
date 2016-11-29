//******Functions Relating to Accelerometers******//

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include <plib/delays.h>
#include <plib/timers.h>
#include <string.h>
#include "AccelerationFuncs.h"
#include "TimingFuncs.h"

// Configure ADC module
void ConfigADC(void)
{
ADCON1=0b00011000;
ADCON2=0b10111111;
ADCON0=0b00010001;
}

//Local accelerometer read functions. These functions read in the output of the ADXL335 accelerometer
//Return ints for quick and accurate serial printing - multiplied by 100 for extra precision, since they return int
int Read_X(float m, float c)
{
    int x;
    ADCON0bits.CHS=0b0110;			//ch6
    Delay10TCYx(100);                           //Delay 1000 instruction cycles (0.25ms)
    ConvertADC();
    while (BusyADC());
    x=(ReadADC()*m-c)*100;                      //Read acceleration with passed in parameters
    //x=ReadADC();                              //Read raw value
    return x;
}

int Read_Y(float m, float c)
{
    int y;
    ADCON0bits.CHS=0b0100;			//ch4
    Delay10TCYx(100);                           //Delay 1000 instruction cycles (0.25ms)
    ConvertADC();
    while (BusyADC());
    y=(ReadADC()*m-c)*100;                      //Read acceleration with passed in parameters
    //y=ReadADC();                              //Read raw value
    return y;
}

int Read_Z(float m, float c)
{
    int z;
    ADCON0bits.CHS=0b0101;			//ch5
    Delay10TCYx(100);                           //Delay 1000 instruction cycles (0.25ms)
    ConvertADC();
    while (BusyADC());
    z=(ReadADC()*m-c)*100;                      //Read acceleration with passed in parameters
    //z=ReadADC();                              //Read raw value
    return z;
}

void ReadAndTime(void)
{
    //Read accelerometer outputs (gives acceleration*100)
    x = Read_X(gscale,goffset);
    y = Read_Y(gscale,goffset);
    z = Read_Z(gscale,goffset);

    //Compute long and short averages for use by PumpDetect
    xlongavg=approxRollingAverage(xlongavg,x,Nlong);
    ylongavg=approxRollingAverage(ylongavg,y,Nlong);
    zlongavg=approxRollingAverage(zlongavg,z,Nlong);

    xshortavg=approxRollingAverage(xshortavg,x,Nshort);
    yshortavg=approxRollingAverage(yshortavg,y,Nshort);
    zshortavg=approxRollingAverage(zshortavg,z,Nshort);

    //Time since last read
    TimeLoop();
}

long int approxRollingAverage (long int avg, long int new_sample, int N)
{
    avg += (new_sample - avg) / N;
    return avg;
}

int PumpDetect(void)
{
    if (offcount>offcountmax+1) // If offcount has grown to maximum value
    {
        offcount=offcountmax+1; // Let it saturate so that it does not overflow
    }
    // If difference between any of the long or short averages is greater...
    // than the treshold, reset counter and return pumping true
    if ((abs(xlongavg-xshortavg)>onthresh)||(abs(ylongavg-yshortavg)>onthresh)||(abs(zlongavg-zshortavg)>onthresh))
    {
        offcount=0;
        return 1;
    }
    else
    {
        if (offcount>offcountmax)
        {
            // Not pumping
            return 0;
        }
        else
        {
            // Not detected pumping, but increment counter and return true
            offcount=offcount++;
            return 1;
        }
    }
}