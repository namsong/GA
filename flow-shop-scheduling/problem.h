#pragma once

/**
    Flow-shop scheduling

    m: jobs
    n: machines or operations

    data: http://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/flowshop1.txt
    refs: http://www.acad.ro/sectii2002/proceedings/doc2012-1/10-Ancau.pdf
    https://www.idc-online.com/technical_references/pdfs/chemical_engineering/SOME%20NOVEL%20METHODS%20FOR%20FLOW%20SHOP%20SCHEDULING.pdf
*/

// problem
typedef struct pro_t pro_t;
struct pro_t {
    int m; // # of jobs
    int n; // # of machines

    int **t; // operation time of

    int *jt; // job finish time, used to evaluate solutions  
    int *mt; // machine finish time, used to evalate solutions  
};

extern pro_t P;

// solution
typedef struct oper_t oper_t;
struct oper_t {
    int st, ft;
};

typedef struct sol_t sol_t;
struct sol_t {
    int *jseq;
    oper_t **oper; // row: machine, col: job
    int makespan;
};

void pro_open(const char *fname);
void pro_close();
sol_t *sol_new();
void sol_del(sol_t **psol);
void sol_decoding(sol_t *sol, float *rnd);
int sol_evaluate(sol_t *sol);
void sol_gantt(sol_t *sol);
