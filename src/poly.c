#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "poly.h"

/* Print a polynomial */
void print_poly_u(poly_u *p) {
    for(uint32_t i=p->deg; i > 0; i--)
        if(p->coef[i] != 0)
            printf("%dX^%d + ", p->coef[i], i);
    printf("%d\n", p->coef[0]);
}

/* Allocate a polynomial of degree deg */
poly_u *alloc_poly_u(uint32_t deg) {
    uint32_t *coef = malloc(sizeof(uint32_t)*(deg+1)); // X**deg is != null
    poly_u *p = malloc(sizeof(uint32_t));
    p->coef = coef;
    p->deg = deg;
    return p;
}

/* Free a polynomial p */
void free_poly_u(poly_u *p) {
    if(p->coef != NULL)
        free(p->coef);
    if(p != NULL)
        free(p);
}

/* Naive polynomial multiplication
 * This should be O(n^2) */
poly_u *mulpu(poly_u *p, poly_u *q) {
    int32_t deg = p->deg + q->deg;
    poly_u *k = alloc_poly_u(deg);
    for (uint32_t i=0; i <= deg; i++)
        k->coef[i] = 0;
    for (uint32_t i=0; i <= p->deg; i++)
        for (uint32_t j=0; j<=(q->deg);j++)
            k->coef[i+j] += p->coef[i] * q->coef[j];
    return k;
}

// TODO
//poly_u *mulpuk1() {
    
//}

