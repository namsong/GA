#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ga.h"

ga_t *ga;

#define PI 3.14159265

double f(double x)
{
    return x * sin(10.*PI * x) + 3.5;
}

void genome_init(genome_t *one)
{
    int i;
	byte *genes = (byte *)one->buff;

    for (i = 0; i < one->length; i++) {
        genes[i] = rand() % 2;
    }
}

void genome_eval(genome_t *one)
{
    double x;
	byte *genes = (byte *)one->buff;

    x = bin2real(genes, one->length, -1.0, 2.0);
    one->value = f(x);
}

void flip_mutation(genome_t *one, double mrate)
{
    int i;
	byte *genes = (byte *)one->buff;

    for (i = 0; i < one->length; i++) {
        if (randp() < mrate) {
            genes[i]++;
            genes[i] %= 2;
        }
    }
}

void genome_print(genome_t *g)
{
    int i;
	byte *genes = (byte *)g->buff;
    double x = bin2real(genes, g->length, -1., 2.);
    double f = g->value;

    printf("%10.6f(%10.6f) : ", x, f);
    for (i = g->length - 1; i >= 0; i--) {
        putchar('0' + genes[i]);
    }
    putchar('\n');
}

void pop_print(pop_t *p)
{
    int g;
    for (g = 0; g < p->psize; g++) {
        genome_print(p->genomes[g]);
    }
}

int main(int argc, char *argv[])
{
    srand( (unsigned)time( NULL ) );

    ga = ga_new(10, sizeof(byte), 19);

    ga->xrate = 0.9;
    ga->mrate = 0.1;
    ga->maxgen = 100;
    ga->maxmin = MAXIMIZE;
    ga->elitism = true;

    ga->genome_init = genome_init;
    ga->genome_eval = genome_eval;
    ga->selection  = roulette_wheel;
    ga->crossover  = one_point_xover;
    ga->mutation   = flip_mutation;

    ga_run(ga);

    genome_print(ga->best);
	printf("\n");

    pop_print(ga->newpop);

    ga_del(&ga);

    getchar();
    return 0;
}


