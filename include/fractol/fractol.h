#ifndef FRACTOL_H
#define FRACTOL_H

#include "fractol/numeric/backend.h"
#include "fractol/fractal_context.h"
#include "pixel_buffer.h"
#include <MLX42/MLX42.h>
#include <stdbool.h>

typedef struct s_fractol {
  t_fractal_context *ctx;
  mlx_image_t       *canvas;
  bool               paused;
} t_fractol;

t_fractol *fractol_create(int width, int height);
void      fractol_destroy(t_fractol *f);

void      fractol_update(t_fractol *f);
void      fractol_render(t_fractol *f);

#endif
