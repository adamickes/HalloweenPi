///////////////////////////////////////////////////////////////////////
//
//	HOWLOWEEN.C									Written by: Adam Ickes
//
//	This program is intended for use only on a Raspberry Pi with the
//	mpg123 program installed. 
//
//	Howloween.c was designed to continuously play random halloween
//	oriented sound effects at random intervals.
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

//Minimum and Maximum wait (in seconds) between sounds
#define RAND_MIN_WAIT 10
#define RAND_MAX_WAIT 25
//Number of SFX files in the directory
#define NUMBER_OF_FILES 90

int gen_rand(int max);

void playRandomSound()
{
	char *file;
	char *fileName;
	char *fileLocation = "/home/pi/sfx/";
	//printf ("test printf");
	
	pid_t x; 
	int fileNumber = gen_rand((int)NUMBER_OF_FILES);
	int i = 0;
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("/home/pi/sfx"); //"." refers to the current dir
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
	//Seed rand() with current time
	srand(time(NULL));
	
    //Process events forever
	int waitTime;
    while (1) {
		//Delay before nextsound is played
		waitTime = gen_rand((int)RAND_MAX_WAIT - (int)RAND_MIN_WAIT);
		sleep((int)RAND_MIN_WAIT + waitTime);
		playRandomSound();
    }
	
	return 0;
}