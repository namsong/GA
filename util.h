#pragma once

#include <stdlib.h>
#include "setup.h"

typedef unsigned char byte;
typedef unsigned short int  word;
typedef unsigned int uint;

inline double randp() { // r0.0 <= p <= 1.0 real random number generator
	return rand()/RAND_MAX;
}
double bin2real(byte *bits, int n, double xmin, double xmax);
