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

void g_init(genome_t *one)
{
    int i;
	byte *genes = (byte *)one->buff;

    for (i = 0; i < one->length; i++) {
        genes[i] = rand() % 2;
    }
}

void g_eval(genome_t *one)
{
    double x;
	byte *genes = (byte *)one->buff;

    x = bin2real(genes, one->length, -1.0, 2.0);
    one->value = f(x);
}

void g_print(const char *title, genome_t *g)
{
    int i;
	byte *genes = (byte *)g->buff;
    double x = bin2real(genes, g->length, -1., 2.);
    double f = g->value;
	
	printf(title);
    printf("%10.6f(%16.13f) : ", x, f);
    for (i = 0; i < g->length; i++) {
        putchar('0' + genes[i]);
    }
    putchar('\n');
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

int main(int argc, char *argv[])
{
    srand( (unsigned)time( NULL ) );

    ga = ga_new(10, sizeof(byte),32);

    ga->xrate = 0.9;
    ga->mrate = 0.1;
    ga->maxgen = 1000;
    ga->maxmin = MAXIMIZE;
    ga->elitism = true;

    genome_init = g_init;
    genome_eval = g_eval;
	genome_print = g_print;
    ga->selection = roulette_wheel;
    ga->crossover = one_point_xover;
    ga->mutation = flip_mutation;
	ga->verbose = 10;

    ga_run(ga);

    pop_print("\nResult:\n", ga->newpop);
	genome_print("best:\n", ga->best);

    ga_del(&ga);

    getchar();
    return 0;
}


