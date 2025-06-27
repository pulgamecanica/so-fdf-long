#ifndef FRACTOL_COLOR_H
#define FRACTOL_COLOR_H

#include "fractol/fractal_context.h"

uint32_t iter_to_color(uint32_t iter, uint32_t max_iter, const t_coord *z, const t_fractal_context *ctx);

#endif
