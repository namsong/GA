#pragma once
#include "pop.h"

// 목적함수 타입
typedef enum {MINIMIZE, MAXIMIZE} obj_t;

// 유전알고리즘(Genetic Algorithm)
typedef struct ga_t {
    pop_t    *newpop;	// 새로운 모집단
    pop_t    *oldpop;	// 이전 모집단
    genome_t *best;     // 생성된 개체 중 가장 좋은 개체
    int       gsize;    // 개체의 유전자 하나의 크기(바이트수)
    int       length;   // 개체의 유전가 개수
    int       psize;    // 모집단 크기
    double    xrate;    // 교차변이 확률
    double    mrate;    // 돌연변이 확률
    int       ngen;     // 세대 교체 수
    int       maxgen;   // 최대 세대 교체 수
    obj_t     maxmin;   // 목적 함수 타입
    bool      elitism;  // 모집단에 가장 좋은 개체를 유지할 것인지 여부
	int       verbose;  // 진행과정을 출력할 최대 세대 교체 수

    void      (*selection)(pop_t *newpop, pop_t *oldpop); // 선택 함수
    void      (*crossover)(genome_t *dad, genome_t *mom); // 교차변이 함수
    void      (*mutation)(genome_t *one, double mrate);	  // 돌연변이 함수
} ga_t;

ga_t *ga_new(int psize, int gsize, int length);
void ga_del(ga_t **ga);
void ga_initialize(ga_t *ga);
void ga_evaluation(ga_t *ga);
void ga_selection(ga_t *ga);
void ga_crossover(ga_t *ga);
void ga_mutation(ga_t *ga);
void ga_run(ga_t *ga);

// schemes
void roulette_wheel(pop_t *newpop, pop_t *oldpop);
void one_point_xover(genome_t *dad, genome_t *mom);

