#include <stdbool.h>
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
    if(p->coef != NULL) // Null may be non portable
        free(p->coef);
    if(p != NULL)
        free(p);
}

/* Return true if p and q are the same (:= same coefs) */
bool equal_poly_u(poly_u *p, poly_u *q) {
    uint32_t deg = p->deg > q->deg ? p->deg : q->deg; // Take the maximum deg
    for(uint32_t i=0; i < deg; i++)  // We compare each coef
        if (i > p->deg && q->coef[i] != 0) // Avoid out of range
            return false;
        else if (i > q->deg && p->coef[i] != 0) // Avoid out of range
            return false;
        else if (p->coef[i] != q->coef[i]) // If they're not the same
            return false;
    return true;
}

/* Return the sum of two polynomials */
poly_u *addpu(poly_u *p, poly_u *q) {
    uint32_t deg = p->deg > q->deg ? p->deg : q->deg; // Take the maximum deg
    poly_u *r = alloc_poly_u(deg);// Result
    for(uint32_t i=0; i<=deg; i++)
        if (i > p->deg) // Avoid out of range error
            r->coef[i] = q->coef[i];
        else if (i > q->deg)  // Same
            r->coef[i] = p->coef[i];
        else // Return the sum at degree i
            r->coef[i] = p->coef[i] + q->coef[i];
    return r;
}

/* Multiply a polynomial by X^n (simplifying computings) */
poly_u *mulpx(uint32_t n, poly_u *p) {
    poly_u *r = alloc_poly_u(p->deg + n); // Result
    for(uint32_t i=0; i <= p->deg + n; i++)
        if (i<n)
            r->coef[i] = 0;
        else
            r->coef[i] = p->coef[i-n];
    return r;
}

/* Cut a polynom from X^n to X^m (division somewhat) */
/* m should be < deg(P) !! */
poly_u *cutpu(uint32_t n, uint32_t m, poly_u *p) {
    poly_u *r = alloc_poly_u(m-n); // Result
    for(uint32_t i=n; i<=m; i++)
        r->coef[i-n] = p->coef[i];
    return r;
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


poly_u *negp(poly_u *p) {
    poly_u *r = alloc_poly_u(p->deg);
    for(uint32_t i=0; i<=p->deg; i++)
        r->coef[i] = -p->coef[i];
    return r;
}

/* Polynomial product with PQ = (P1X^n + P0)(Q1X^n + Q0) */
/* Non-recursive decomposition */
poly_u *mulpuk1(poly_u *p, poly_u *q) {
    // We cut each polynomial in half
    poly_u *p0 = cutpu(0, p->deg/2, p);
    poly_u *p1 = cutpu(p->deg/2, p->deg, p);
    poly_u *q0 = cutpu(0, q->deg/2, q);
    poly_u *q1 = cutpu(q->deg/2, q->deg, q); 
    uint32_t n = p0->deg + 1;
    // We do every operation 1 per 1
    poly_u *a = mulpu(p0, q0);
    poly_u *ppp = addpu(p0, p1);
    poly_u *qpq = addpu(q0, q1);
    poly_u *b = mulpu(ppp, qpq);
    poly_u *c = mulpu(p1, q1);
    poly_u *cx = mulpx(2*n, c);
    poly_u *apc = addpu(a, c);
    polt_y *mapc = negp(a, c);
    poly_u *bmapc = addpu(b, mapc); // B - (A+C)
    poly_u *bmapcx = mulpx(n, bmapc); // On appelle ca h pour simplifier
    poly_u *hpa = addpu(bmapcx, a);
    poly_u *result = 

    // TIME TO FREE EVERYTHING..................
    free_poly_u(a);
    free_poly_u(b);
    free_poly_u(c);
    free_poly_u(p0);
    free_poly_u(q0);
    free_poly_u(p1);
    free_poly_u(q1);
    free_poly_u(ppp);
    free_poly_u(qpq);
    free_poly_u(cx);
    free_poly_u(apc);
    free_poly_u(bmapc);
}
