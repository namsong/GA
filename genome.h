#pragma once
#include "setup.h"

/* Genome type */
typedef struct genome_t genome_t;
struct genome_t {
    gene_t	*genes;
    double   value;
    int      length;
};

/* Genome functions */
genome_t *genome_new(int length);
void genome_del(genome_t **genome);
void genome_copy(genome_t *dst, genome_t *src);
