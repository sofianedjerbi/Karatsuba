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
    printf("    -k1: 1 decomposition multiplication (mulpk1)\n");
    printf("    -k: recursive decomposition multiplication (mulpkr)\n");
    printf("    -t: tests\n");
}

int main(int argc, char **argv) {
    if (argc != 2)
        print_options();
    else if (strcmp(argv[1], "-n") == 0)  // mulpu
        test_multiply(mulpu, 500000, 100000);
    else if (strcmp(argv[1], "-k") == 0) // mulpuk1
        test_multiply(mulpukr, 500000, 100000);
    else if (strcmp(argv[1], "-k1") == 0) // mulpuk1
        test_multiply(mulpuk1, 500000, 100000);
    else if (strcmp(argv[1], "-t") == 0){ // Tests
        compare_multiply(mulpuk1); // Make sure that mulpuk1 works
        compare_multiply(mulpukr);
    }
    else
        print_options();
    return 0;
}

