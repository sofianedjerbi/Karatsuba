#pragma once
#include <stdint.h>

#include "poly.h"

// Benchmark multiply algorithms
void test_multiply(poly_u *(multiply)(poly_u *, poly_u *),
                   uint32_t, uint32_t);

