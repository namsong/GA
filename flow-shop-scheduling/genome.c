#include <stdlib.h>
#include <memory.h>
#include "genome.h"

/* genome 생성
 * length : 길이
 */
genome_t *genome_new(int length)
{
    genome_t *genome;

    genome = (genome_t *)malloc(sizeof(genome_t));
    genome->genes = (gene_t *)malloc(sizeof(gene_t) * length);
    genome->value = 0;
    genome->length = length;

    return genome;
}

/* genome 해제 */
void genome_del(genome_t **genome)
{
	if(genome && *genome) {
		free((*genome)->genes);
		free(*genome);
		*genome = NULL;
	}
}

/* genome 복사 */
void genome_copy(genome_t *dst, genome_t *src)
{
    memcpy(dst->genes, src->genes, sizeof(gene_t)*src->length);
    dst->value = src->value;
}

