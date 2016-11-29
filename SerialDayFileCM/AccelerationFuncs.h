/* 
 * File:   AccelerarionFuncs.h
 * Author: Ben
 *
 * Created on 12 September 2015, 14:08
 */

#ifndef ACCELERARIONFUNCS_H
#define	ACCELERARIONFUNCS_H

//Variables
float goffset=557.5;                    //Constants for acceleration calculation
float gscale=1.1;


//Constats to change for thresholds & off timing
unsigned long int offcount = 0;
unsigned int offcountmax = 2000;
int onthresh = 2000;
int Nlong = 50;
int Nshort = 10;

int x;                         //Accelerations
int y;
int z;

long int xlongavg=0;
long int ylongavg=0;
long int zlongavg=0;
long int xshortavg=0;
long int yshortavg=0;
long int zshortavg=0;


// Configure ADC module
void ConfigADC(void);

//Local accelerometer read functions. These functions read in the output of the ADXL335 accelerometer
//Return ints for quick and accurate serial printing - multiplied by 100 for extra precision
int Read_X(float, float);
int Read_Y(float, float);
int Read_Z(float, float);
void ReadAndTime(void);
long int approxRollingAverage (long int, long int, int);
int PumpDetect(void);


#endif	/* ACCELERARIONFUNCS_H */