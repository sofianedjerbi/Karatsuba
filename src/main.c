#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "poly.h"
#include "tests.h"

void print_options(){
    printf("Options:\n");
    printf("    -n: Naive multiplication (mulpu)\n");
}

int main(int argc, char **argv) {
    poly_u *q = alloc_poly_u(5);
    poly_u *p = alloc_poly_u(4);
    uint32_t c[] = {0,1,2,3,4};
    uint32_t d[] = {0,1,1,1,1,1};
    p->coef = c;
    q->coef = d;
    poly_u *g = addpu(p,q);
    print_poly_u(p);
    print_poly_u(q);
    print_poly_u(g);
    
    /*
    if (argc != 2)
        print_options();

    else if (strcmp(argv[1], "-n") == 0)
        test_multiply(mulpu, 20000, 1000);
    
    return 0;
    */
}

