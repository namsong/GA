#pragma once
#include "genome.h"

typedef struct pop_t pop_t;
struct pop_t {
    genome_t **genomes; // genome 포인터 배열
    int        psize;	// 모집단 크기
};

pop_t *pop_new(int psize, int gsize, int length);
void pop_del(pop_t **p);
void pop_print(const char *title, pop_t *p);

