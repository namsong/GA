#pragma once
#include "genome.h"

typedef struct pop_t pop_t;
struct pop_t {
    genome_t **genomes;
    int        size;
    int        best;
    int        worst;
};

pop_t *pop_new(int size, int length);
void pop_del(pop_t **pop);

