#pragma once
#include "util.h"

// 게놈(genome), 개체(individual), 유전자(chromosome), 스트링
typedef struct genome_t genome_t;
struct genome_t {
    void	*buff;
    int      gsize;
    int      length;
    double   value;
};

// 게놈 함수 포인터
extern void (*genome_init)(genome_t *one);	// 개체 초기화 함수
extern void (*genome_eval)(genome_t *one);	// 개체 평가 함수
extern void (*genome_print)(const char *title, genome_t *one); // 개체 출력 함수

// 게놈 함수
void *genome_gene(genome_t *g, int i);
genome_t *genome_new(int gsize, int length);
void genome_del(genome_t **genome);
void genome_copy(genome_t *dst, genome_t *src);

