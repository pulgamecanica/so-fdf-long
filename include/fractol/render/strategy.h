#ifndef FRACTOL_RENDER_STRATEGY_H
#define FRACTOL_RENDER_STRATEGY_H

#include "fractol/fractal_context.h"

typedef void (*t_render_fn)(t_fractal_context *ctx);

extern t_render_fn g_render_strategies[];

#endif
