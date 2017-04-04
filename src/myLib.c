#include "myLib.h"

/**
 * delay(int n)
 * Runs a blocking loop.
 * @param n How long to run loop for
 */
void delay(int n) {
	int volatile x = 0;
	for(int i =0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

/**
 * keySensitiveDelay(int n)
 * Runs a blocking loop that checks for key presses during its process.
 * @param n How long to run loop for
 * @return  A merged bitvector of all key presses
 */
u32 keySensitiveDelay(int n) {
	u32 keys = 0;
	int volatile x = 0;
	for(int i =0; i<n*30; i++)
	{
		x = x + 1;
		keys |= ~(BUTTONS);
	}

	return keys;
}

/**
 * waitForVBlank()
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 159);
}

// Random Number Generator
// Courtesy of Tonc
int __qran_seed= 42;
int sqran(int seed) {
    int old= __qran_seed;
    __qran_seed= seed;
    return old;
}
int qran() {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

/**
 * qran_range(int min, int max)
 * Generates a pseudo-random number between min and max, inclusive.
 * @param  min bottom end of range.
 * @param  max top end of range.
 * @return     random number in the given range.
 */
int qran_range(int min, int max) {
	return (qran()*(max-min)>>15)+min;
}
