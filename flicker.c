///////////////////////////////////////////////////////////////////////
//
//	FLICKER.C				Written by: Adam Ickes
//
//	This program is intended for use only on a Raspberry Pi with the
//	wiringPi library installed. 
//
//	Flicker.c was designed to make the signal to a single GPIO pin, as
//	defined by constant FLICKER_PIN, flicker on and off at random. The 
//  	orinigal intention for this code was to be used as part of a 
//	Halloween setup to make lights installed in ground fog flicker 
//	randomly.
//
///////////////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Maximum flickers in a sequence
#define RAND_MAX_FLICKERS 8
//Minimum and Maximum wait (in milliseconds) between flickers in sequence
#define RAND_MIN_FLICKER_WAIT 10
#define RAND_MAX_FLICKER_WAIT 180
//Minimum and Maximum wait (in seconds) between flicker sequences
#define RAND_MIN_WAIT 2
#define RAND_MAX_WAIT 8
//Flicker pin number
#define FLICKER_PIN 7

int gen_rand(int max);

void flicker() 
{
	int numberOfFlickers = gen_rand((int)RAND_MAX_FLICKERS);
	int i = 1;
	int waitTime;
	
	//Turn Off Pin
	digitalWrite(FLICKER_PIN, 1);
	
	while (i <= numberOfFlickers){
		//Delay before light on
		waitTime = gen_rand((int)RAND_MAX_FLICKER_WAIT - (int)RAND_MIN_FLICKER_WAIT);
		delay((int)RAND_MIN_FLICKER_WAIT + waitTime);
		
		//Turn On Pin
		digitalWrite(FLICKER_PIN, 0);
		
		//Delay before light off
		waitTime = gen_rand((int)RAND_MAX_FLICKER_WAIT - (int)RAND_MIN_FLICKER_WAIT);
		delay((int)RAND_MIN_FLICKER_WAIT + waitTime);
		
		//Turn Off Pin
		digitalWrite(FLICKER_PIN, 1);
		i = i + 1;
	}
	
	//Turn On Pin
	digitalWrite(FLICKER_PIN, 0);
}

int gen_rand(int max)
{
	int n;
	n=rand()%max;
	return(n);
}

int main() 
{

	//Start as a daemon
    	if (daemon(0,0) != 0) {
		printf("Start program using command 'sudo halloween/flicker'");
		exit(1);
    	}
    
    	//Setup wiringPi
    	if (wiringPiSetup() == -1) {
		printf("wiringPi is not setup on this Raspberry Pi'");
		exit(1);
    	}
		
		//Set flicker pin mode to out
		pinMode(FLICKER_PIN, OUTPUT);
		
		//Turn on flicker pin
		digitalWrite(FLICKER_PIN, 0);
		
		//Seed rand() with current time
		srand(time(NULL));
	
    	//Process events forever
    	int waitTime;
    	while (1) {
		//Delay before flickering sequence begins
		waitTime = gen_rand((int)RAND_MAX_WAIT - (int)RAND_MIN_WAIT);
		delay(((int)RAND_MIN_WAIT + waitTime)*1000);
		flicker();
    	}
	
	return 0;
}