#pragma once
#include "pop.h"

typedef enum {MINIMIZE, MAXIMIZE} obj_t;

/* Genetic Algorithm */
typedef struct ga_t {
    pop_t    *newpop;
    pop_t    *oldpop;
    genome_t *best;     /* best genome */
    int       length;   /* the length of genome */
    int       size;     /* population size */
    double    xrate;    /* crossover length, default = 0.9 */
    double    mrate;    /* mutation length, default = 0.01 */
    int       ngen;     /* the number of generations */
    int       maxgen;   /* the maximum number of generations, default = 10000 */
    obj_t     maxmin;   /* maximize or minimize problem, default = MAXIMIZE */
    bool      elitism;  /* use elitism strategy or not, default = true*/

    void      (*genome_init)(genome_t *one);
    void      (*genome_eval)(genome_t *one);
    void      (*selection)(pop_t *newpop, pop_t *oldpop);
    void      (*crossover)(genome_t *dad, genome_t *mom);
    void      (*mutation)(genome_t *one, double mrate);
} ga_t;

ga_t *ga_new(int size, int length);
void ga_del(ga_t **ga);
void ga_initialize(ga_t *ga);
void ga_evaluation(ga_t *ga);
void ga_selection(ga_t *ga);
void ga_crossover(ga_t *ga);
void ga_mutation(ga_t *ga);
void ga_run(ga_t *ga);

