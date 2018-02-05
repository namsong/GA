#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ga.h"
#include "problem.h"

ga_t *ga;
sol_t *S;

void genome_init(genome_t *g)
{
    int i;
    float *genes = (float *)g->buff;

    for (i = 0; i < g->length; i++) {
        genes[i] = (float)randp();
    }
}

void genome_eval(genome_t *g)
{
    float *genes = (float *)g->buff;
    sol_decoding(S, genes);
    g->value = sol_evaluate(S);
}

void randp_mutation(genome_t *g, double mrate)
{
    float *genes = (float *)g->buff;
    int i;

    for (i = 0; i < g->length; i++) {
        if (randp() < mrate) {
            genes[i] = (float)randp();
        }
    }
}

void genome_print(genome_t *g)
{
    int i;

    genome_eval(g);
    
    printf("(%10.6f) : ", g->value);
    for (i = 0; i < P.jobs; i++) {
        printf("%d ", S->jseq[i]);
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
	const char *fname = "IPCAR_4.txt";
	int psize = 10;		 // population size
	double xrate = 0.90; // crossover rate
	double mrate = 0.05; // mutation rate
	int maxgen = 10000;  // maximum generation: stop condition
	bool gantt = false;

	switch (argc) {
	case 7: gantt = (argv[6][0] == 'y');
	case 6: maxgen = atoi(argv[5]);
	case 5: mrate = atof(argv[4]);
	case 4: xrate = atof(argv[3]);
	case 3: psize = atoi(argv[2]);
	case 2: fname = argv[1];
	}

    srand( (unsigned)time( NULL ) );

    pro_open(fname);
    S = sol_new();

    ga = ga_new(10, sizeof(float), P.jobs);
	ga->xrate = xrate;
	ga->mrate = mrate;
	ga->maxgen = maxgen;
	ga->maxmin = MINIMIZE;
    ga->elitism = true;

    ga->genome_init = genome_init;
    ga->genome_eval = genome_eval;
    ga->selection  = roulette_wheel;
    ga->crossover  = one_point_xover;
    ga->mutation   = randp_mutation;

    ga_run(ga);

    genome_print(ga->best);
    printf("\n");
	if (gantt) sol_gantt(S);

    pop_print(ga->newpop);

    ga_del(&ga);

    sol_del(&S);
    pro_close();
    return 0;
}


