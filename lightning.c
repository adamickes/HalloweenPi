///////////////////////////////////////////////////////////////////////
//
//	LIGHTNING.C				Written by: Adam Ickes
//
//	This program is intended for use only on a Raspberry Pi with the
//	wiringPi library installed. 
//
//	Lightning.c was designed to make the signal to a single GPIO pin, as
//	defined by constant FLICKER_PIN, flicker on and off at random to
//	simulate lightning and accompany the flickering light with a random
//	lightning sound. The orinigal intention for this code was to be used 
//	as part of a Halloween setup.
//
///////////////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

//Maximum flickers in a sequence
#define RAND_MAX_FLICKERS 7
//Minimum and Maximum wait (in milliseconds) between flickers in sequence
#define RAND_MIN_FLICKER_WAIT 20
#define RAND_MAX_FLICKER_WAIT 150
//Minimum and Maximum wait (in seconds) between flicker sequences
#define RAND_MIN_WAIT 15
#define RAND_MAX_WAIT 30
//Flicker pin number
#define FLICKER_PIN 6
//Number of SFX files in the directory
#define NUMBER_OF_FILES 11

int gen_rand(int max);

void lightning() 
{
	int numberOfFlickers = gen_rand((int)RAND_MAX_FLICKERS);
	int i = 1;
	int waitTime;
	
	//Turn On Pin
	digitalWrite(FLICKER_PIN, 0);
	
	while (i <= numberOfFlickers){
		//Delay before light off
		waitTime = gen_rand((int)RAND_MAX_FLICKER_WAIT - (int)RAND_MIN_FLICKER_WAIT);
		delay((int)RAND_MIN_FLICKER_WAIT + waitTime);
		
		//Turn Off Pin
		digitalWrite(FLICKER_PIN, 1);
		
		//Delay before light on
		waitTime = gen_rand((int)RAND_MAX_FLICKER_WAIT - (int)RAND_MIN_FLICKER_WAIT);
		delay((int)RAND_MIN_FLICKER_WAIT + waitTime);
		
		//Turn On Pin
		digitalWrite(FLICKER_PIN, 0);
		i = i + 1;
	}
	
	//Turn Off Pin
	digitalWrite(FLICKER_PIN, 1);
}

void thunder()
{
	char *file;
	char *fileName;
	char *fileLocation = "/home/pi/thunder/";
	//printf ("test printf");
	
	pid_t x; 
	int fileNumber = gen_rand((int)NUMBER_OF_FILES);
	int i = 0;
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("/home/pi/thunder"); //"." refers to the current dir
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (i==fileNumber) {
			fileName = pent->d_name;
			//printf ("%s\n",fileName);
		}
		i = i + 1;
	}
	if (errno){
		printf ("readdir() failure; terminating");
		exit(1);
	}
	closedir(pdir);
	
	//Combine fileLocation and fileName
	file = (char *)malloc(strlen(fileLocation) + strlen(fileName) +1);
	strcpy(file,fileLocation);
	strcat(file,fileName);
	//Play the sound
	x = fork();
	execlp("mpg123","mpg123",file);
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
		
		//Turn off flicker pin
		digitalWrite(FLICKER_PIN, 1);
		
		//Seed rand() with current time
		srand(time(NULL));
	
    	//Process events forever
    	int waitTime;
    	while (1) {
		//Delay before flickering sequence begins
		waitTime = gen_rand((int)RAND_MAX_WAIT - (int)RAND_MIN_WAIT);
		delay(((int)RAND_MIN_WAIT + waitTime)*1000);
		lightning();
		delay(250);
		thunder();
    	}
	
	return 0;
}