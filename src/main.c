#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "tests.h"

int main(int argc, char *argv[]) {
    
    printf("Test mulpu:\n");
    test_multiply(mulpu, 100000, 10000);
}
