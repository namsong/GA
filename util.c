#include <stdlib.h>
#include "util.h"

// 0.0 <= p <= 1.0 범위의 랜덤넘버 생성
double randp()
{ 
	return (double)rand()/RAND_MAX;
}

// 2진수가 저장된 문자배열을 실수로 변환
//  bits : 2진수 배열의 시작 포인터
//     n : 2진수 자리수
//  xmin : 최소값
//  xmax : 최대값
double bin2real(byte *bits, int n, double xmin, double xmax)
{
    double x = 0.;
    double e = 1.;
    int i;

    for (i = 0; i < n; i++, e *= 2.) {
        if (bits[i]) {
            x += e;
        }
    }

    x = xmin + x * (xmax - xmin) / (e - 1.);
    return x;
}
