#include <stdio.h>
#include <stdlib.h>
#include "pop.h"

// 모집단 생성
// size : 모집단 크기
// length : genome의 길이
pop_t *pop_new(int psize, int gsize, int length)
{
    pop_t *pop;
    int i;

    pop = (pop_t *)malloc(sizeof(pop_t));
    pop->genomes = (genome_t **)malloc(sizeof(genome_t *)*psize);
    pop->psize = psize;

    for (i = 0; i < psize; i++) {
        pop->genomes[i] = genome_new(gsize, length);
    }

    return pop;
}

// 모집단 해제
void pop_del(pop_t **pop)
{
    if (pop && *pop) {
        pop_t *p = *pop;
        int i;

        for (i = 0; i < p->psize; i++) {
            genome_del(&p->genomes[i]);
        }
        free(p->genomes);
        free(p);
        *pop = NULL;
    }
}

void pop_print(const char *title, pop_t *p)
{
	printf(title);
	if(genome_print){
		int i;
		for (i = 0; i < p->psize; i++) {
			genome_print("",p->genomes[i]);
		}
	}
	printf("\n");
}

