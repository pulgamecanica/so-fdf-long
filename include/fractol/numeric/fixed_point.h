#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

typedef int64_t fixed_t;

#define FIXED_FRAC_BITS 32
#define FIXED_ONE ((fixed_t)1 << FIXED_FRAC_BITS)

fixed_t fixed_from_double(double d);
double  fixed_to_double(fixed_t f);
fixed_t fixed_mul(fixed_t a, fixed_t b);
fixed_t fixed_square(fixed_t a);

#endif
