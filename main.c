#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ga.h"
#include "util.h"

ga_t *ga;

#define PI 3.14159265

double f(double x)
{
    return x * sin(10.*PI * x) + 3.5;
}

void InitGenome(genome_t *one)
{
    int i;

    for (i = 0; i < one->length; i++) {
        one->genes[i] = rand() % 2;
    }
}

void EvalGenome(genome_t *one)
{
    double x;

    x = bin2real(one->genes, one->length, -1.0, 2.0);
    one->value = f(x);
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
            one->genes[i]++;
            one->genes[i] %= 2;
        }
    }
}

void PrintGenome(genome_t *g)
{
    int i;
    double x = bin2real(g->genes, g->length, -1., 2.);
    double f = g->value;

    printf("%10.6f(%10.6f) : ", x, f);
    for (i = g->length - 1; i >= 0; i--) {
        putchar('0' + g->genes[i]);
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
    srand( (unsigned)time( NULL ) );

    ga = ga_new(10, 19);

    ga->xrate = 0.9;
    ga->mrate = 0.1;
    ga->maxgen = 100;
    ga->maxmin = MAXIMIZE;
    ga->elitism = true;

    ga->genome_init = InitGenome;
    ga->genome_eval = EvalGenome;
    ga->selection  = RouletteWheel;
    ga->crossover  = OnePointXover;
    ga->mutation   = FlipMutation;

    ga_run(ga);

    double x = bin2real(ga->best->genes, ga->best->length, -1., 2.);
    double f = ga->best->value;

    PrintGenome(ga->best);
    PrintPop(ga->newpop);

    ga_del(&ga);

    getchar();
    return 0;
}


