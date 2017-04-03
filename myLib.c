#include "myLib.h"

void delay(int n) {
	int volatile x = 0;
	for(int i =0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

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

void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 159);
}

// RNG here
int __qran_seed= 42;     // Seed / rnd holder

// Seed routine
int sqran(int seed)
{
    int old= __qran_seed;
    __qran_seed= seed;
    return old;
}

//! Quick (and very dirty) pseudo-random number generator
/*! \return random in range [0,8000h>
*/
int qran()
{
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int qran_range(int min, int max)
{    return (qran()*(max-min)>>15)+min;     }
