HalloweenPi
===========

Raspberry Pi C programs for use in my Halloween display


flicker.c - sets one GPIO pin to flicker randomly. I will be using it for lighting installed in ground hugging fog. (requires the wiringPi library)
	compile with: gcc -o flicker flicker.c -lwiringPi
	run with: sudo ./flicker


howloween.c - plays random Halloween themed .wav files at random intervals (not included).
	compile with: gcc -o howloween howloween.c
	run with: ./howloween
