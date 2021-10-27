#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "poly.h"
#include "tests.h"

/* Met au carré le polynome X^n + ... + 1
 * Itérativement de degré 1 jusqu'au degré max_deg
 * Synopsis: Mesurer les perfs d'une fonction de multiplication 
 * Params: 
 *  - multiply: fonction de multiplication (pointeur) 
 *  - max_deg: Degré de fin du test */
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


