#include <stdlib.h>
#include "ga.h"
#include "util.h"

/* GA */
ga_t *ga_new(int size, int length)
{
    ga_t *ga;

    ga = (ga_t *)malloc(sizeof(ga_t));
    ga->newpop = pop_new(size, length);
    ga->oldpop = pop_new(size, length);
    ga->best = genome_new(length);
    ga->length = length;
    ga->size = size;

    /* default parameter */
    ga->xrate = 0.9;
    ga->mrate = 0.01;
    ga->maxgen = 10000;
    ga->maxmin = MAXIMIZE;
    ga->elitism = true;

    return ga;
}

void ga_del(ga_t **ga)
{
    if (ga && *ga) {
        ga_t *g = *ga;
        pop_del(&g->newpop);
        pop_del(&g->oldpop);
        genome_del(&g->best);
        free(g);
        *ga = NULL;
    }
}

void ga_initialize(ga_t *ga)
{
    int i;

    for (i = 0; i < ga->size; i++) {
        ga->genome_init(ga->newpop->genomes[i]);
    }
    ga->genome_eval(ga->best);
}

bool better(ga_t *ga, genome_t *a, genome_t *b)
{
    switch (ga->maxmin) {
    case MAXIMIZE:
        if (a->value > b->value) return true;
        break;
    case MINIMIZE:
        if (a->value < b->value) return true;
        break;
    }
    return false;
}

void ga_evaluation(ga_t *ga)
{
    int i, b, w;
    genome_t **genomes = ga->newpop->genomes;

    b = w = 0;
    ga->genome_eval(genomes[0]);
    for (i = 1; i < ga->size; i++) {
        ga->genome_eval(genomes[i]);
        if (better(ga, genomes[i], genomes[b])) b = i;
        if (better(ga, genomes[w], genomes[i])) w = i;
    }
    ga->newpop->best = b;
    ga->newpop->worst = w;

    if (better(ga, genomes[b], ga->best)) {
        genome_copy(ga->best, genomes[b]);
    } else if (ga->elitism
               && genomes[b]->value != ga->best->value) { /* if use elitism strategy */
        genome_copy(genomes[w], ga->best); /* change the worst genome with the best genome */
    }
}

void ga_selection(ga_t *ga)
{
    pop_t *tmppop = ga->newpop;
    ga->newpop = ga->oldpop;
    ga->oldpop = tmppop;

    ga->selection(ga->newpop, ga->oldpop);
}

void ga_crossover(ga_t *ga)
{
    int i, dad, mom;

    dad = mom = -1;
    for (i = 0; i < ga->size; i++) {
        if (randp() > ga->xrate) continue;
        if (dad < 0) dad = i;
        else {
            mom = i;
            ga->crossover(ga->newpop->genomes[dad], ga->newpop->genomes[mom]);
            dad = mom = -1;
        }
    }
}

void ga_mutation(ga_t *ga)
{
    int i;

    for (i = 0; i < ga->size; i++) {
        ga->mutation(ga->newpop->genomes[i], ga->mrate);
    }
}

void ga_run(ga_t *ga)
{
    ga_initialize(ga);
    ga_evaluation(ga);
    for (ga->ngen = 0; ga->ngen < ga->maxgen; ga->ngen++) {
        ga_selection(ga);
        ga_crossover(ga);
        ga_mutation(ga);
        ga_evaluation(ga);
    }
}


