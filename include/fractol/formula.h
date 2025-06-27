#ifndef FRACTOL_FORMULA_H
#define FRACTOL_FORMULA_H

#include <stdint.h>
#include "fractol/fractal_context.h"
#include "fractol/numeric/backend.h"

typedef uint32_t (*t_fractal_formula)(const t_coord *z, t_fractal_context *ctx);

#endif
