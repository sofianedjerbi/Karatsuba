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
    uint32_t *c = malloc(sizeof(uint32_t)*(deg+1)); // X**deg != null
    poly_u *p = malloc(sizeof(poly_u));
    p->coef = c;
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
    uint32_t deg = MAX(p->deg, q->deg); // Take the maximum deg
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
void addpu(poly_u *p, poly_u *q, poly_u *r) {
    uint32_t deg = MAX(p->deg, q->deg);
    for(uint32_t i=0; i<=deg; i++)
        if (i > p->deg) // Avoid out of range error
            r->coef[i] = q->coef[i];
        else if (i > q->deg)  // Same
            r->coef[i] = p->coef[i];
        else // Return the sum at degree i
            r->coef[i] = p->coef[i] + q->coef[i];
}

/* Multiply a polynomial by X^n (simplifying computings) */
void mulpx(uint32_t n, poly_u *p, bool rea) {
    // Realloc more memory to handle all the coefficients
    if (rea)
        p->coef = realloc(p->coef, sizeof(uint32_t) * (p->deg + n + 1)); 
    // Left shift by n
    for(int64_t i=p->deg; i >= 0; i--){
        p->coef[i+n] = p->coef[i];
        p->coef[i] = 0;
    }
    p->deg += n; // Change degree
}

/* Cut a polynom from X^n to X^m (division somewhat) and 
 * put the result into q */
/* m should be < deg(P) !! */
void cutpu(uint32_t n, uint32_t m, poly_u *p, poly_u *q) {
    q->deg = m-n;
    q->coef = p->coef + n; // Start at the nth element
}

/* Naive polynomial multiplication
 * This should be O(n^2) */
poly_u *mulpu(poly_u *p, poly_u *q) {
    uint32_t deg = p->deg + q->deg;
    poly_u *k = alloc_poly_u(deg);
    for (uint32_t i=0; i <= deg; i++)
        k->coef[i] = 0;
    for (uint32_t i=0; i <= p->deg; i++)
        for (uint32_t j=0; j<=(q->deg);j++)
            k->coef[i+j] += p->coef[i] * q->coef[j];
    return k;

}

/* The opposite of a polynomial */
void negp(poly_u *p) {    
    for(uint32_t i=0; i<=p->deg; i++)
        p->coef[i] = -p->coef[i];
}

/* Multiply a polynomial by an int */
poly_u *cst_multiply(uint32_t k, poly_u *p) {
    poly_u *r = alloc_poly_u(p->deg);
    for(uint32_t i=0; i<=p->deg; i++)
        r->coef[i] = k * p->coef[i];
    return r;
}


/* Polynomial product with PQ = (P1X^n + P0)(Q1X^n + Q0) */
/* Non-recursive decomposition */
poly_u *mulpuk1(poly_u *p, poly_u *q) {
    if (p->deg == 0)
        return cst_multiply(p->coef[0], q);
    else if (q->deg == 0)
        return cst_multiply(q->coef[0], p);

    // We cut each polynomial in half
    // p0 p1 q0 q1 are aliased to p and q
    poly_u p0, p1, q0, q1;
    cutpu(0, p->deg/2, p, &p0);
    cutpu(p->deg/2+1, p->deg, p, &p1);
    cutpu(0, q->deg/2, q, &q0);
    cutpu(q->deg/2+1, q->deg, q, &q1);  
    uint32_t n = p0.deg + 1;

    poly_u ppp, qpq; // addition vars
    // local coefs on the stack
    ppp.deg = MAX(p0.deg, p1.deg);
    qpq.deg = MAX(q0.deg, q1.deg);
    uint32_t pp[ppp.deg]; 
    uint32_t qq[qpq.deg];
    ppp.coef = pp;
    qpq.coef = qq;

    // We do every operation 1 per 1
    addpu(&p0, &p1, &ppp); // P0 + P1
    addpu(&q0, &q1, &qpq); // Q0 + Q1

    poly_u *a = mulpu(&p0, &q0);
    poly_u *c = mulpu(&p1, &q1);
    poly_u *b = mulpu(&ppp, &qpq); 

    // Placeholder for additions
    uint32_t deg = MAX(MAX(a->deg, c->deg), b->deg); // Max(a,b,c)
    poly_u *r = alloc_poly_u(p->deg + q->deg);
    r->deg = deg;

    addpu(a, c, r);   // T = A+C
    negp(r);          // T = -A-C
    addpu(b, r, r);   // T = B - (A+C)
    mulpx(n, r, false);      // T = (B-(A+C))*X^n
    addpu(a, r, r);   // T = (B-(A+C))*X^n + A
    mulpx(2*n, c, true);    // C = CX^2n
    // Defining degree and adding result
    
    r->deg = p->deg + q->deg;
    
    addpu(c, r, r);   // R = (B-(A+C))*X^n + A + CX^2n

    // free everything
    free_poly_u(a);
    free_poly_u(b);
    free_poly_u(c);
    
    return r;
}

/* Wrapper for mulpkrt */ 
poly_u *mulpukr(poly_u *p, poly_u *q) { 
    return mulpukrt(p, q, DEG_THRESHOLD); 
}

/* Multiplication with iterative decomposition and custom threshold */
poly_u *mulpukrt(poly_u *p, poly_u *q, uint32_t deg_threshold) {
    if (p->deg == 0)
        return cst_multiply(p->coef[0], q);
    else if (q->deg == 0)
        return cst_multiply(q->coef[0], p);

    // If we trigger the 1 decomposition calculation
    if (p->deg + q->deg < deg_threshold){
        return mulpuk1(p, q);
    }

    // We cut each polynomial in half
    // p0 p1 q0 q1 are aliased to p and q
    poly_u p0, p1, q0, q1;
    cutpu(0, p->deg/2, p, &p0);
    cutpu(p->deg/2+1, p->deg, p, &p1);
    cutpu(0, q->deg/2, q, &q0);
    cutpu(q->deg/2+1, q->deg, q, &q1);  
    uint32_t n = p0.deg + 1;

    poly_u ppp, qpq; // addition vars
    // local coefs on the stack
    ppp.deg = MAX(p0.deg, p1.deg);
    qpq.deg = MAX(q0.deg, q1.deg);
    uint32_t pp[ppp.deg]; 
    uint32_t qq[qpq.deg];
    ppp.coef = pp;
    qpq.coef = qq;

    // We do every operation 1 per 1
    addpu(&p0, &p1, &ppp); // P0 + P1
    addpu(&q0, &q1, &qpq); // Q0 + Q1

    poly_u *a = mulpukrt(&p0, &q0, deg_threshold);
    poly_u *c = mulpukrt(&p1, &q1, deg_threshold);
    poly_u *b = mulpukrt(&ppp, &qpq, deg_threshold); 

    // Placeholder for additions
    uint32_t deg = MAX(MAX(a->deg, c->deg), b->deg); // Max(a,b,c)
    poly_u *r = alloc_poly_u(p->deg + q->deg);
    r->deg = deg;
    addpu(a, c, r);   // T = A+C
    negp(r);          // T = -A-C
    addpu(b, r, r);   // T = B - (A+C)
    mulpx(n, r, false);      // T = (B-(A+C))*X^n
    addpu(a, r, r);   // T = (B-(A+C))*X^n + A
    mulpx(2*n, c, true);    // C = CX^2n
    // Defining degree and adding result
    
    r->deg = p->deg + q->deg;
    addpu(c, r, r);   // R = (B-(A+C))*X^n + A + CX^2n

    // free everything
    free_poly_u(a);
    free_poly_u(b);
    free_poly_u(c);
    
    return r;
}


