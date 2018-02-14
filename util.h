#pragma once

#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned short int  word;
typedef unsigned int uint;

double randp(); // 1 이하의 임의의 실수
double bin2real(byte *bits, int n, double xmin, double xmax); // 2진 문자  배열을 실수로 변환
