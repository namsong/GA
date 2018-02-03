#include <stdlib.h>
#include "pop.h"

/* 모집단 생성
 *   size : 모집단 크기
 * length : genome의 길이
 */
pop_t *pop_new(int size, int length)
{
    pop_t *pop;
    int i;

    pop = (pop_t *)malloc(sizeof(pop_t));
    pop->genomes = (genome_t **)malloc(sizeof(genome_t *)*size);
    pop->size = size;

    for (i = 0; i < size; i++) {
        pop->genomes[i] = genome_new(length);
    }

    return pop;
}

/* 모집단 해제 */
void pop_del(pop_t **pop)
{
    if (pop && *pop) {
        pop_t *p = *pop;
        int i;

        for (i = 0; i < p->size; i++) {
            genome_del(&p->genomes[i]);
        }
        free(p->genomes);
        free(p);
        *pop = NULL;
    }
}


