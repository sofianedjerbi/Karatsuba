#pragma once
#include <stdint.h>

// Represent a polynom of degree deg
typedef struct poly_u {
    uint32_t *coef;  // Coef list
    uint32_t deg;    // Polynom degree
} poly_u;

// Print a polynom
void print_poly_u(poly_u *);

// Allocate a polynom
poly_u *alloc_poly_u(uint32_t);

// Free a polynom
void free_poly_u(poly_u *);

// Naive polynom multiplication
poly_u *mulpu(poly_u *, poly_u *);
