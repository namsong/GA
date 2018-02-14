#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ga.h"

// GA 생성
ga_t *ga_new(int psize, int gsize, int length)
{
    ga_t *ga;

    ga = (ga_t *)malloc(sizeof(ga_t));
    ga->newpop = pop_new(psize, gsize, length);
    ga->oldpop = pop_new(psize, gsize, length);
    ga->best = genome_new(gsize,length);
    ga->gsize = gsize;
    ga->length = length;
    ga->psize = psize;

    // default parameter 
    ga->xrate = 0.9;
    ga->mrate = 0.01;
    ga->maxgen = 10000;
    ga->maxmin = MAXIMIZE;
    ga->elitism = true;

	// default scheme 
	ga->selection = roulette_wheel;
	ga->crossover = one_point_xover;

    return ga;
}

// GA 파괴
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

// 모집단을 초기화한다.
void ga_initialize(ga_t *ga)
{
    int i;

    for (i = 0; i < ga->psize; i++) {
        genome_init(ga->newpop->genomes[i]);
    }
    genome_eval(ga->best);
	
}

// 두 개체의 적응도 비교
// 두 개체의 평가함수 값과 목적함수 형태를 고려한다.
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

// 평가
// 모집단의 개체를 모두 평가한다.
// 엘리티즘도 구현한다.
void ga_evaluation(ga_t *ga)
{
    int i;
    genome_t **genomes = ga->newpop->genomes;
	genome_t *best;

    genome_eval(genomes[0]);
	best = genomes[0];
    for (i = 1; i < ga->psize; i++) {
        genome_eval(genomes[i]);
        if (better(ga, genomes[i], best)) best = genomes[i];
    }
    if (better(ga, best, ga->best)) {
        genome_copy(ga->best, best);
    }
}

// 자연선택
// 적응력에 따라 다음 세대를 생산할 개체들을 선별한다.
void ga_selection(ga_t *ga)
{
    pop_t *tmppop = ga->newpop;
	
	ga->newpop = ga->oldpop;
    ga->oldpop = tmppop;

    ga->selection(ga->newpop, ga->oldpop);

	if(ga->elitism) { // find the best and the worst
		genome_t **genomes = ga->newpop->genomes;
		genome_t *best, *worst;
		int i;

		best = ga->newpop->genomes[0];
		worst = ga->newpop->genomes[0];
		for (i = 1; i < ga->psize; i++) {
			if (better(ga, genomes[i], best)) best = genomes[i];
			if (better(ga, worst, genomes[i])) worst = genomes[i];
		}
		
		if (better(ga, ga->best, best)) { // and the best genome was destroyed,  
			genome_copy(worst, ga->best); // change the worst genome with the best genome 
		}
	}
}

// 교차변이
// 적응력에 따라 자연선택된 부모 모집단에서
// 주어진 교차확률에 따라 확률적으로 선택된 부모 개체는 
// 짝짓기를 하여 새로운 자손을 생산하고 생산된 자손은 부모 개체를 대체한다.
void ga_crossover(ga_t *ga)
{
    int i, dad, mom;

    dad = mom = -1;
    for (i = 0; i < ga->psize; i++) {
        if (randp() > ga->xrate) continue;
        if (dad < 0) dad = i;
        else {
            mom = i;
            ga->crossover(ga->newpop->genomes[dad], ga->newpop->genomes[mom]);
            dad = mom = -1;
        }
    }
}

// 돌연변이
// 교차변이 단계를 거친 세대의 개체의 유전형질에 
// 주어진 돌연변이 확률로 돌연변이가 발생한다.
void ga_mutation(ga_t *ga)
{
    int i;

    for (i = 0; i < ga->psize; i++) {
        ga->mutation(ga->newpop->genomes[i], ga->mrate);
    }
}

// GA 를 수행한다.
void ga_run(ga_t *ga)
{
	if(!genome_init) {
		fprintf(stderr,"ERROR: genome_init is not set up.\n");
		exit(-1);
	}
	if(!genome_eval) {
		fprintf(stderr,"ERROR: genome_eval is not set up.\n");
		exit(-1);
	}

	ga_initialize(ga);
    ga_evaluation(ga);
	if (0 < ga->verbose) {
		pop_print("init population:\n", ga->newpop);
	}
    for (ga->ngen = 0; ga->ngen < ga->maxgen; ga->ngen++) {
        ga_selection(ga);
		if(ga->ngen < ga->verbose) {
			printf("%d-th generation --------------\n",ga->ngen+1);
			pop_print("selection:\n",ga->newpop);
		}
        ga_crossover(ga);
		if(ga->ngen < ga->verbose) {
			ga_evaluation(ga);
			pop_print("crossover:\n",ga->newpop);
		}
        ga_mutation(ga);
		if(ga->ngen < ga->verbose) {
			ga_evaluation(ga);
			pop_print("mution:\n",ga->newpop);
		}
        ga_evaluation(ga);
		if(ga->ngen < ga->verbose) {
			genome_print("best:\n", ga->best);
			printf("\n");
		}
    }
}

// 유틸리티 함수들

// 룰렛휠 선별함수
void roulette_wheel(pop_t *newpop, pop_t *oldpop)
{
	double   *probs;
	double    sum, r;
	int i, j;

	probs = (double *)malloc(sizeof(double) * oldpop->psize);

	sum = 0.;
	for (i = 0; i < oldpop->psize; i++) {
		probs[i] = oldpop->genomes[i]->value;
		sum += probs[i];
	}

	probs[0] /= sum;
	for (i = 1; i < oldpop->psize; i++) {
		probs[i] /= sum;
		probs[i] += probs[i - 1];
	}

	for (i = 0; i < oldpop->psize; i++) {
		r = randp();
		for (j = 0; j < oldpop->psize - 1; j++) {
			if (probs[j] >= r) break;
		}
		genome_copy(newpop->genomes[i], oldpop->genomes[j]);
	}

	free(probs);
}

// 1 점 교차변이
void one_point_xover(genome_t *dad, genome_t *mom)
{
	void *t;
	int x;
	int size;

	x = rand() % dad->length;
	size = dad->gsize * x;

	t = calloc(x, size);
	memcpy(t, dad->buff, size);
	memcpy(dad->buff, mom->buff, size);
	memcpy(mom->buff, t, size);

	free(t);
}

