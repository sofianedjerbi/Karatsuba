#include <stdbool.h>
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
    for (uint32_t d=0; d <= max_deg; d+=inc){
        poly_u *p = alloc_poly_u(d);  // Squared polynomial
        for (uint32_t i=0; i <= d; i++) // Init p
            p->coef[i] = 1;
        start = clock();
        poly_u *k = (*multiply)(p, p); // We multiply here
        end = clock();
        time = ((double)(end-start))/CLOCKS_PER_SEC;
        printf("Time for degree %d: %lf\n", d, time);
        free_poly_u(p);
        free_poly_u(k);
    }
}

/* Compute the square of the polynomial nX^n + ... + 0
 * From degree 1 to degree 1000 and compare with mulpu. 
 * Synopsis: Check if the multiplication operation is correct 
 * Params: 
 *  - multiply: multiplycation func (pointer) */
void compare_multiply(poly_u *(*multiply)(poly_u *, poly_u *)) {
    printf("beginning test\n");
    for (uint32_t d=0; d <= 3; d++){
        poly_u *p = alloc_poly_u(d);  // Squared polynomial
        
        for (uint32_t i=0; i <= d; i++) // Initialize p
            p->coef[i] = 1;
        
        poly_u *k = (*multiply)(p, p); // We multiply here
        poly_u *l = mulpu(p, p);
        if (!equal_poly_u(k, l)){
            printf("ERROR\n");
            printf("polynomial p:  ");
            print_poly_u(p);
            printf("p^2 mulpu:     "); // Error
            print_poly_u(l);
            printf("p^2 result:    ");
            print_poly_u(k);
        }
        free_poly_u(p);
        free_poly_u(k);
        free_poly_u(l);
    }
    printf("end test\n");
}
