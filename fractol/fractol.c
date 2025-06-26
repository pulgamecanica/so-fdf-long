#include "fractol/fractol.h"
#include "fractol/color.h"
#include "fractol/render/strategy.h"
#include "mlx_utils.h"
#include "app.h"
#include <stdlib.h>

extern t_app g_app;

t_fractol *fractol_create(int width, int height) {
  t_fractol *f = malloc(sizeof(*f));
  if (!f)
    return NULL;

  f->ctx = fractol_create_context(width, height);
  if (!f->ctx) {
    free(f);
    return NULL;
  }

  f->canvas = mlx_new_image(g_app.mlx, width, height);
  if (!f->canvas) {
    fractol_destroy_context(f->ctx);
    free(f);
    return NULL;
  }

  f->paused = false;
  return f;
}

void fractol_destroy(t_fractol *f) {
  if (!f) return;
  if (f->canvas)
    mlx_delete_image(g_app.mlx, f->canvas);
  fractol_destroy_context(f->ctx);
  free(f);
}

void fractol_update(t_fractol *f) {
  if (!f || f->paused)
    return;

  g_render_strategies[f->ctx->render_strategy](f->ctx);
}

void fractol_render(t_fractol *f) {
  if (!f || !f->canvas)
    return;

  t_pixel_buffer *buf = f->ctx->buffer;
  for (int y = 0; y < buf->height; ++y) {
    for (int x = 0; x < buf->width; ++x) {
      int idx = y * buf->width + x;
      uint32_t iter = buf->data[idx].iterations;
      uint32_t color = iter_to_color(iter, f->ctx->max_iterations, f->ctx);
      mlx_put_pixel(f->canvas, x, y, color);
    }
  }

  mlx_image_to_window(g_app.mlx, f->canvas, 0, 0);
}
