#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include <plib/delays.h>
#include <plib/timers.h>
#include "SerialFuncs.h"
#include "InterruptFuncs.h"
#include "AccelerationFuncs.h"
#include "TimingFuncs.h"

// AM Modified
#include "fft.h"
// Define FFT length
#define cmNFFT 128
// Define filter order
#define cmFilterOrder 4
// Define weightVector length
#define cmWeightLen 21


//-----------------PIC18LF4520 pragmas--------------------------
#pragma config WDT = OFF		//Watch-dog timer off
#pragma config OSC = HSPLL		//Crystal oscillator with PLL (16MHz) ("OCS=HS" for 4MHz Crystal)
#pragma config LVP = OFF		//Single-supply ICSP disabled
#pragma config PBADEN = OFF             //PORTB<4:0> digital I/O on reset
#pragma config BOREN = OFF		//Brown-Out reset disabled
/*
#define LogPower LATAbits.LATA0         //Use THERM1 to turn logger on
//#define LogPower LATCbits.LATC0         //Use ON_KEY to turn logger on
#define Indicator LATAbits.LATA1        //Use THERM2 as indicator
#define SerialSource LATBbits.LATB3     //Use DSR to select Openlog(0) or BT(1)
#define BTPower LATBbits.LATB4          //Use CTS to turn BT on
#define OpenLog 1
#define BlueTooth 0
*/

int Pumping=1;
int DayCount=0;

// AM Modified
int cmWinLen = 0;
short cmYreal[cmNFFT]; //= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
short cmYimag[cmNFFT]; //= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// Assign the following weight vector obtained from trained classifier
short cmWeightVector[cmWeightLen] = {-9855, -21, 9, 40, 5, -3, 2, 5, 3, 2, 2, -2, -2, -2, -3, 0, 2, -1, -2, 1, -3};
// yOut will hold the classifier test output per analysis window
int cmYout = 0;
// Wrapper function related to CM stuffs
void cmStuffsWrapperFun();
// Function to compute HPF using phase-corrected LPF(moving average)
void cmFilterHighPassUsingLowPass();
// Wrapper function to compute fft using fft.c
void cmFftFun();

void main(int argc, char** argv) {
    
    TRISA=0xFC;                         //THERM1 and THERM2 outputs
    TRISBbits.RB3=0;                    //DSR set as output
    TRISBbits.RB4=0;                    //CTS set as output
    TRISCbits.RC0=0;                    //ON_KEY pin set as output
    LogPower=0;
    BTPower=1;
    // AM Modified: Comment Bluetooth source
    //SerialSource=BlueTooth;
    SerialSource=OpenLog;
    
    InitUSART();
    InitInterrupts();
    InitTimer0();
    ConfigADC();

    while(Pumping)
    {
        ReadAndTime();
        Pumping = PumpDetect();
        //printf("Pumping=%1i\r\n",Pumping);
        //printf("%i,%i,%i,", x, y, z);
        //printf("%u,%u,%u\r\n", tr, tl, tt);
    }    

    while(1) //infinite loop
    {
        if (SerialSource==BlueTooth && minutes > 10) //Timeout for bluetooth module
        {
            SerialSource=OpenLog;
            BTPower = 0;
        }
        ReadAndTime();
        Pumping=PumpDetect();
        if (Pumping)
        {
            if (SerialSource==OpenLog)
            {
                //Power Logger On
                LogPower=1;
                //Wait until ready
                Delay10KTCYx(200);
                Delay10KTCYx(200);
                Delay10KTCYx(200);

                //Conditions to change file name depending on number of digits
                //in number of days. Should be able to replace with only the
                //Daycount>100 but have not tested this.

                if(DayCount<10)
                {
                    //If new day, new file, then append that file
                    if (NewDay)
                    {
                        DayCount++;
                        NewDay=0;
                        printf("new FILE%1i.txt\r\n",DayCount);
                        Delay10KTCYx(200);
                        Delay10KTCYx(200);
                    }
                    //Else just append file
                    printf("append FILE%1i.txt\r\n",DayCount);
                    Delay10KTCYx(200);
                    Delay10KTCYx(200);
                }
                else if (DayCount<100)
                {
                    //If new day, new file,append file
                    if (NewDay)
                    {
                        DayCount++;
                        NewDay=0;
                        printf("new FILE%2i.txt\r\n",DayCount);
                        Delay10KTCYx(200);
                        Delay10KTCYx(200);
                    }
                    //Else just append file
                    printf("append FILE%2i.txt\r\n",DayCount);
                    Delay10KTCYx(200);
                    Delay10KTCYx(200);
                }
                else if (DayCount<1000)
                {
                    //If new day, new file,append file
                    if (NewDay)
                    {
                        DayCount++;
                        NewDay=0;
                        printf("new FILE%3i.txt\r\n",DayCount);
                        Delay10KTCYx(200);
                        Delay10KTCYx(200);
                    }
                    //Else just append file
                    printf("append FILE%3i.txt\r\n",DayCount);
                    Delay10KTCYx(200);
                    Delay10KTCYx(200);
                }
            }

            //Start of log text
            printf("\r\r\nLog started!\r\n");
            printf("%2i:%2i:%2i\r\n",hours,minutes,seconds);
            printf("%2i/%2i/%4i\r\n\n",day,month,year);

            while (Pumping)
            {
                //Log Accelerations
                ReadAndTime();
                // AM Modified 14/11/2016 Comment following printf
                //printf("%6i,%6i,%6i,%5u\r\n", x, y, z,tt);

                //printf("%6i,",offcount);
                //printf("%6i,%6i",xlongavg,xshortavg);
                //printf("%6i,%6i",ylongavg,yshortavg);
                //printf("%6i,%6i\r\n",zlongavg,zshortavg);
                
                // AM Modified 29/11/2016 Perform all CM-related stuff
                // Filtering, Fft, Testing, and recording
                cmStuffsWrapperFun();
                    
                //Detect if still pumping
                Pumping=PumpDetect();
            }
            
            printf("Pumping Stopped.\r\n");
            printf("%2i:%2i:%2i\r\n",hours,minutes,seconds);
            printf("%2i/%2i/%4i\r\n\n",day,month,year);
            printf("~~~");
            Delay10KTCYx(200);
            Delay10KTCYx(200);
            LogPower=0;
            Delay10KTCYx(200);
            Delay10KTCYx(200);
        }

        //Messages only received while pumping is false.
        if (NewMessage==1)
        {
            MessageRoutine();
        }
    }
}

void cmStuffsWrapperFun(){
// AM Modified 29/11/2016 On-pump CM stuff
    if (cmWinLen<cmNFFT)
    {
        // Record z, y, z, tt
        printf("%6i,%6i,%6i,%5u\r\n", x, y, z,tt);
        // Store the values until we have a window of samples                                
        cmYreal[cmWinLen] = y;
        cmWinLen = cmWinLen+1;
    }
    else
    {       
        // Perform on-pump CM 
        // Comment following when deploying in field
//         printf("Performing on-pump CM...\n");
        // Print inputs (real,imag) to HPF/fft
        // Comment following for loop prints when deploying in field
//         printf("Input signal...\n");
//         for (int iTemp=0;iTemp<cmNFFT;iTemp++){                        
//             printf("\t%i %6i %6i\n",iTemp,cmYreal[iTemp],cmYimag[iTemp]);
//         }

        // Filter using HPF = Signal - LPF(Phase-corrected moving average)
        // Comment following when deploying in field
//        printf("\tComputing HPF...\n");
        cmFilterHighPassUsingLowPass();
        // Comment following for loop prints when deploying in field
//        for (int iTemp=0;iTemp<cmNFFT;iTemp++){                        
//            printf("\t%i %6i\n",iTemp,cmYreal[iTemp]);
//        }

        // Compute FFT
        // Comment following when deploying in field
//        printf("\tComputing FFT...\n");
        cmFftFun();
        // Print magnitude of fft
        // Comment following for loop prints when deploying in field
//        for (int iTemp=0;iTemp<cmNFFT/2;iTemp++){
//            printf("\t%i %6i\n",iTemp,cmYreal[iTemp]);
//        }

        // Test classifier: yOut=Xtest*w
        // Comment following when deploying in field
//        printf("\tTesting classifier...\n");
        cmYout = 1*cmWeightVector[0];
        // Comment following for loop prints when deploying in field
        printf("\t%i %i %i\n",1,cmWeightVector[0],cmYout);
        for (int iW=1,iTemp=2;iW<cmWeightLen;iW++,iTemp=iTemp+3){
            // If using |FFT|
//            cmYout = cmYout+cmYreal[iTemp]*cmWeightVector[iW];    
            // If using 10*log10(|FFT|)
            cmYout = cmYout+cmWeightVector[iW]*10*log(cmYreal[iTemp])/log(10); 
            // Comment following when deploying in field
//            printf("\t%i %i %i\n",cmYreal[iTemp],cmWeightVector[iW],cmYout);
        }          

        // Record z, y, z, tt, and yOut
        printf("%6i,%6i,%6i,%5u,%i\r\n", x, y, z,tt,cmYout);

        cmWinLen = 0;
    }    
}

void cmFilterHighPassUsingLowPass(){
// Filter parameters
	// Ad-hoc: Use 4th order filter with following filter coeff
	int cmFilterCoeff[cmFilterOrder] = {1, 1, 1, 1};

	// To prevent phase-shift append zeros to input signal before filtering
	// Number of zeros to append = filterOrder/2
	// Make sure filterOrder is even!
	int Nsignal = sizeof(cmYreal)/sizeof(cmYreal[0]);

	// Moving average filter
	// Following loops haven't been verified for filterOrder!=4
	int xBuffer = 0;
	for (int i=cmFilterOrder/2; i<Nsignal+cmFilterOrder/2; i++){
		int xLPF = 0;
		if (i+1<cmFilterOrder){
			for (int j=i, k=0; j>=0; k++,j--){
				xLPF = xLPF + cmFilterCoeff[k]*cmYreal[j];
//				printf("%d \t %d %d \t %d %d \t %d %d\n",i,k,filterCoeff[k],j,realNumbers[j],i-2,xLPF);
			}
		}
		else{
			for (int j=0; j<cmFilterOrder; j++){
				if(i-j<Nsignal){
					xLPF = xLPF + cmFilterCoeff[j]*cmYreal[i-j];
				}
//				printf("%d \t %d %d \t %d %d \t %d %d\n",i,j,filterCoeff[j],i-j,realNumbers[i-j],i-2,xLPF);
			}
		}
		if (i>cmFilterOrder/2){
			// Ad-hoc!
			// Compute HPF for the previous point
			//xHPF = x-xLPF;
			cmYreal[i-3] = cmFilterOrder*cmYreal[i-3] - xBuffer;			
//			printf("%d \t %d %d\n",i,i-3,cmYreal[i-3]);
		}
        xBuffer = xLPF;
	}
	// Ad-hoc!
	// Compute HPF for the end point
	cmYreal[Nsignal-1] = cmFilterOrder*cmYreal[Nsignal-1] - xBuffer;
//	printf("%d \t %d %d\n",Nsignal+cmFilterOrder/2-1,Nsignal-1,cmYreal[Nsignal-1]);    
}

void cmFftFun(){
    // AM Modified: Removed PIC port set up    
	
    // AM Modified: Removed what I think that has to do with drawing fft graph
    
    // AM Modified: Remove infinite loop
		// Perform the FFT        

        // AM Modified: imaginaryNumbers and realNumbers defined above
//		// Get 64 samples at 50uS intervals
//		// 50uS means our sampling rate is 20KHz which gives us
//		// Nyquist limit of 10Khz

		// Perform the (forward) FFT calculation

		// Note: the FFT result length is half of the input data length
		// so if we put 64 samples in we get 32 buckets out.  The first bucket
		// cannot be used so in reality our result is 31 buckets.
		//
		// The maximum frequency we can measure is half of the sampling rate
		// so if we sample at 20Khz our maximum is 10Khz (this is called the 
		// Nyquist frequency).  So if we have 32 buckets divided over 10Khz,
		// each bucket represents 312.5Khz of range, so our lowest bucket is
		// (bucket 1) 312.5Hz - 625Hz and so on up to our 32nd bucket which
		// is 9687.5Hz - 10,000Hz
		
		//  1 : 312.5 - 625
		//  2 : 625 - 937.5
		//  3 : 937.5 - 1250
		//  4 : 1250 - 1562.5
		//  5 : 1562.5 - 1875
		//  6 : 1875 - 2187.5
		//  7 : 2187.5 - 2500
		//  8 : 2500 - 2812.5
		//  9 : 2812.5 - 3125
		// 10 : 3125 - 3437.5
		// 11 : 3437.5 - 3750
		// 12 : 3750 - 4062.5
		// 13 : 4062.5 - 4375
		// 14 : 4375 - 4687.5
		// 15 : 4687.5 - 5000
		// 16 : 5000 - 5312.5
		// 17 : 5312.5 - 5625
		// 18 : 5625 - 5937.5
		// 19 : 5937.5 - 6250
		// 20 : 6250 - 6562.5
		// 21 : 6562.5 - 6875
		// 22 : 6875 - 7187.5
		// 23 : 7187.5 - 7500
		// 24 : 7500 - 7812.5
		// 25 : 7812.5 - 8125
		// 26 : 8125 - 8437.5
		// 27 : 8437.5 - 8750
		// 28 : 8750 - 9062.5
		// 29 : 9062.5 - 9375
		// 30 : 9375 - 9687.5
		// 31 : 9687.5 - 10000
		
		// Note: the '6' is the size of the input data (2 to the power of 6 = 64)
        // AM Modified: Removed pin assignment
        //		TESTPIN_W5 = 1;
    
        short sizeData = log(cmNFFT)/log(2);
		fix_fft(cmYreal, cmYimag, sizeData);
		
        // AM Modified: Print
        // printf("\nInside fftFun \nComputing FFTs...");
    
		// Take the absolute value of the FFT results
		
		// Note: The FFT routine returns 'complex' numbers which consist of
		// both a real and imaginary part.  To find the 'absolute' value
		// of the returned data we have to calculate the complex number's
		// distance from zero which requires a little pythagoras and therefore
		// a square-root calculation too.  Since the PIC has multiplication
		// hardware, only the square-root needs to be optimised.          
		long place, root;
        for (int k=0; k < cmNFFT/2; k++)
        {
	        cmYreal[k] = (cmYreal[k] * cmYreal[k] + 
                   cmYimag[k] * cmYimag[k]);
                   
            // Now we find the square root of realNumbers[k] using a very
            // fast (but compiler dependent) integer approximation:
            // (adapted from: http://www.codecodex.com/wiki/Calculate_an_integer_square_root)
            place = 0x40000000;
			root = 0;
			
			if (cmYreal[k] >= 0) // Ensure we don't have a negative number
			{
				while (place > cmYreal[k]) place = place >> 2; 
				
				while (place)  
				{  
					if (cmYreal[k] >= root + place)  
					{  
						cmYreal[k] -= root + place;  
						root += place * 2;  
					}  
					root = root >> 1;  
					place = place >> 2;  
				}
			}
			cmYreal[k] = root;            
	    }
        // AM Modified: Commented pin assignment
	    
	    // Now we have 32 buckets of audio frequency data represented as
	    // linear intensity in realNumbers[]
	    //
	    // Since the maximum input value (in theory) to the SQRT function is
	    // 32767, the peak output at this stage is SQRT(32767) = 181.
	    
        // AM Modified: Comment drawing bar graph
        // AM Modified: Commented pin assignment
}
