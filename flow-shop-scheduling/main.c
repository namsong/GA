#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ga.h"
#include "util.h"
#include "problem.h"

ga_t *ga;
sol_t *S;

void InitGenome(genome_t *one)
{
    int i;

    for (i = 0; i < one->length; i++) {
        one->genes[i] = randp();
    }
}

void EvalGenome(genome_t *one)
{
    sol_decoding(S, one->genes);
    one->value = sol_evaluate(S);
}

void RouletteWheel(pop_t *newpop, pop_t *oldpop)
{
    double   *probs;
    double    sum, r;
    int i, j;

    probs = (double *)malloc(sizeof(double) * oldpop->size);

    sum = 0.;
    for (i = 0; i < oldpop->size; i++) {
        probs[i] = oldpop->genomes[i]->value;
        sum += probs[i];
    }

    probs[0] /= sum;
    for (i = 1; i < oldpop->size; i++) {
        probs[i] /= sum;
        probs[i] += probs[i - 1];
    }

    for (i = 0; i < oldpop->size; i++) {
        r = randp();
        for (j = 0; j < oldpop->size - 1; j++) {
            if (probs[j] >= r) break;
        }
        genome_copy(newpop->genomes[i], oldpop->genomes[j]);
    }

    free(probs);
}

void OnePointXover(genome_t *dad, genome_t *mom)
{
    genome_t *P1, *P2;
    int i, x;

    P1 = genome_new(dad->length);
    P2 = genome_new(mom->length);
    genome_copy(P1, dad);
    genome_copy(P2, mom);

    x = rand() % dad->length;
    for (i = x; i < dad->length; i++) {
        dad->genes[i] = P2->genes[i];
        mom->genes[i] = P1->genes[i];
    }
}

void FlipMutation(genome_t *one, double mrate)
{
    int i;

    for (i = 0; i < one->length; i++) {
        if (randp() < mrate) {
            one->genes[i] = randp();
        }
    }
}

void PrintGenome(genome_t *g)
{
    int i;

    EvalGenome(g);
    
    printf("(%10.6f) : ", g->value);
    for (i = 0; i < P.m; i++) {
        printf("%d ", S->jseq[i]);
    }
    putchar('\n');
}

void PrintPop(pop_t *p)
{
    int g;
    for (g = 0; g < p->size; g++) {
        PrintGenome(p->genomes[g]);
    }
}

int main(int argc, char *argv[])
{
    const char *fname = "IPCAR_4.txt";
    
    if (1 < argc) {
        fname = argv[1];
    }

    srand( (unsigned)time( NULL ) );

    pro_open(fname);
    S = sol_new();

    ga = ga_new(10, P.m);

    ga->xrate = 0.9;
    ga->mrate = 0.05;
    ga->maxgen = 10000;
    ga->maxmin = MINIMIZE;
    ga->elitism = true;

    ga->genome_init = InitGenome;
    ga->genome_eval = EvalGenome;
    ga->selection  = RouletteWheel;
    ga->crossover  = OnePointXover;
    ga->mutation   = FlipMutation;

    ga_run(ga);

    PrintGenome(ga->best);
    printf("\n");
//    sol_gantt(S);

    PrintPop(ga->newpop);

    ga_del(&ga);

    sol_del(&S);
    pro_close();
    return 0;
}


