﻿#pragma once

#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned short int  word;
typedef unsigned int uint;

double randp();
double bin2real(byte *bits, int n, double xmin, double xmax);
