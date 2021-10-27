#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "poly.h"
#include "tests.h"

/* Compute the square of the polynomial X^n + ... + 1
 * From degree 1 to degree max_deg with incrementation inc
 * Synopsis: Benchmarking 
 * Params: 
 *  - multiply: multiplycation func (pointer) 
 *  - max_deg: maximum degree */
void test_multiply(poly_u *(*multiply)(poly_u *, poly_u *), 
                   uint32_t max_deg, uint32_t inc) {
    clock_t start, end;
    double time;
    for (uint32_t d=0; d < max_deg; d+=inc){
        poly_u *p = alloc_poly_u(d);  // Polynome qu'on va mettre au carré
        for (uint32_t i=0; i < d+1; i++) // Initialiser p
            p->coef[i] = 1;
        start = clock();
        poly_u *k = (*multiply)(p, p); // On multiplie ici
        end = clock();
        time = ((double)(end-start))/CLOCKS_PER_SEC;
        printf("Time for degree %d: %lf\n", d, time);
        free_poly_u(p);
    }
}


