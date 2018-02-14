﻿#pragma once
#include "util.h"

// 게놈(genome), 개체(individual), 유전자(chromosome), 스트링
typedef struct genome_t genome_t;
struct genome_t {
    void	*buff;
    int      gsize;
    int      length;
    double   value;
};

// 게놈 함수
void *genome_gene(genome_t *g, int i);
genome_t *genome_new(int gsize, int length);
void genome_del(genome_t **genome);
void genome_copy(genome_t *dst, genome_t *src);
