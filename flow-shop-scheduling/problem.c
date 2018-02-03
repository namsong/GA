/**
Flow-shop scheduling

m: jobs
n: machines or operations

data: http://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/flowshop1.txt
refs: http://www.acad.ro/sectii2002/proceedings/doc2012-1/10-Ancau.pdf
      https://www.idc-online.com/technical_references/pdfs/chemical_engineering/SOME%20NOVEL%20METHODS%20FOR%20FLOW%20SHOP%20SCHEDULING.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include "problem.h"

pro_t P; // problem: global var.

void pro_open(const char *fname)
{
    FILE *fptr;
    int m, n;
    int i, j;

    fptr = fopen(fname, "rt");
    i = fscanf(fptr, "%d %d", &m, &n);
    while (i < 2) {
        fscanf(fptr, "%*[^\n]%*c");
        i = fscanf(fptr, "%d %d", &m, &n);
    }
    P.m = m;
    P.n = n;
    P.t = (int **)malloc(sizeof(int *)*m);
    P.t[0] = (int *)malloc(sizeof(int)*m*n);
    for (i = 1; i < m; i++) {
        P.t[i] = P.t[i - 1] + n;
    }

    for (i = 0; i < m; i++) { // for all jobs
        for (j = 0; j < n; j++) { // for all machines
                                  // read the operation time of j-th operation in i-th job.
            fscanf(fptr, "%*d %d", &P.t[i][j]);
        }
    }
    fclose(fptr);

    P.jt = (int *)malloc(sizeof(int)*m);
    P.mt = (int *)malloc(sizeof(int)*n);
}

void pro_close()
{
    if (P.t) {
        free(P.t[0]);
        free(P.t);
        P.t = NULL;
        P.m = 0;
        P.n = 0;
    }
}

sol_t *sol_new()
{
    int i;
    sol_t *sol = (sol_t *)malloc(sizeof(sol_t));
    sol->jseq = (int *)malloc(sizeof(int)*P.m);
    sol->oper = (oper_t **)malloc(sizeof(oper_t *)*P.n);
    sol->oper[0] = (oper_t *)malloc(sizeof(oper_t)*P.n*P.m);
    for (i = 1; i < P.n; i++) {
        sol->oper[i] = sol->oper[i - 1] + P.m;
    }

    return sol;
}

void sol_del(sol_t **psol)
{
    if (psol && *psol) {
        sol_t *sol = *psol;
        free(sol->jseq);
        free(sol->oper[0]);
        free(sol->oper);
        free(sol);
        *psol = NULL;
    }
}

#define max(a,b) (((a) > (b))? (a):(b))

// calc makespan
int sol_evaluate(sol_t *sol)
{
    int i;
    int st, ft;
    int job, mch;

    memset(P.jt, 0, sizeof(int)*P.m);
    memset(P.mt, 0, sizeof(int)*P.n);

    for (i = 0; i < P.m; i++) { // for job sequence index
        job = sol->jseq[i];
        for (mch = 0; mch < P.n; mch++) { // for operations
            st = max(P.jt[job], P.mt[mch]); // start time
            ft = st + P.t[job][mch];        // finish time
            sol->oper[mch][i].st = st;    // start time
            sol->oper[mch][i].ft = ft;    // finish time
            P.jt[job] = ft; // finish time for i-th job
            P.mt[mch] = ft; // finish time for j-th machine
        }
    }
    sol->makespan = P.mt[P.n - 1];

    return sol->makespan;
}

// random number -> sequence
void sol_decoding(sol_t *sol, float *rnd)
{
    int i, j;
    int x;

    // init sequence
    for (i = 0; i < P.m; i++) {
        sol->jseq[i] = i;
    }
    // sort index
    for (i = 1; i < P.m; i++) {
        x = sol->jseq[i];
        for (j = i - 1; 0 <= j; j--) {
            if (rnd[x] < rnd[sol->jseq[j]]) {
                sol->jseq[j + 1] = sol->jseq[j];
            } else break;
        }
        sol->jseq[j + 1] = x;
    }
}

// gantt chart
void sol_gantt(sol_t *sol)
{
    int i, j, k;
    int ft;

    for (i = 0; i < P.n; i++) { // for machines
        ft = 0;
        printf("M%02d :", i % 16);
        for (j = 0; j < P.m; j++) { // for jobs
            for (k = ft; k < sol->oper[i][j].st; k++) { // idle
                putchar(' ');
            }
            ft = sol->oper[i][j].ft;
            for (k = sol->oper[i][j].st; k < ft; k++) {
                if (sol->jseq[j] < 10) {
                    printf("%c", '0' + sol->jseq[j]);
                } else {
                    printf("%c", 'A' + sol->jseq[j] - 10);
                }
            }
        }
        putchar('\n');
    }
    putchar('\n');
}

