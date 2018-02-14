﻿#include <stdlib.h>
#include <memory.h>
#include "genome.h"

// gene 의 위치
void *genome_gene(genome_t *g, int i)
{
	return (void *)((byte *)g->buff + g->gsize*i);
}

// genome 생성
// length : 길이
genome_t *genome_new(int gsize, int length)
{
    genome_t *genome;

    genome = (genome_t *)malloc(sizeof(genome_t));
    genome->buff = calloc(length, gsize);
    genome->value = 0;
    genome->gsize = gsize;
    genome->length = length;

    return genome;
}

// genome 해제
void genome_del(genome_t **genome)
{
	if(genome && *genome) {
		free((*genome)->buff);
		free(*genome);
		*genome = NULL;
	}
}

// genome 복사
void genome_copy(genome_t *dst, genome_t *src)
{
    memcpy(dst->buff, src->buff, src->gsize*src->length);
    dst->gsize = src->gsize;
    dst->length = src->length;
    dst->value = src->value;
}

