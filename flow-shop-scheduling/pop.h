#pragma once
#include "genome.h"

typedef struct pop_t pop_t;
struct pop_t {
    genome_t **genomes; // genome 포인터 배열
    int        psize;	// 모집단 크기
    int        best;	// 가장 좋은 개체 위치
    int        worst;	// 가장 나쁜 개체 위치
};

pop_t *pop_new(int psize, int gsize, int length);
void pop_del(pop_t **pop);

