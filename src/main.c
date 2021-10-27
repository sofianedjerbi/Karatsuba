#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "tests.h"

int main(int argc, char *argv[]) {
    
    printf("Test mulpu:\n");
    test_multiply(mulpu, 100000, 1000);
    /*
    poly_u *p = alloc_poly_u(2);
    
    p->coef[0] = 3;
    p->coef[1] = 2;
    p->coef[2] = 3;
    
    poly_u *q = alloc_poly_u(2);
    q->coef[0] = 1;
    q->coef[1] = 2;
    q->coef[2] = 3;
    
    poly_u *k = mulpu(p, q);
    
    print_poly_u(k);
    print_poly_u(p);
        
    free_poly_u(p);
    free_poly_u(q);
    free_poly_u(k);
    */
}

