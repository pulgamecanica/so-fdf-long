#include "fractol/fractal_context.h"
#include <stdlib.h>
#include <string.h>

t_fractal_context *fractol_create_context(int width, int height) {
  extern t_precision_backend g_long_double_backend;

  t_fractal_context *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  memset(ctx, 0, sizeof *ctx);
  ctx->buffer = pixel_buffer_create(width, height);
  if (!ctx->buffer) {
    free(ctx);
    return NULL;
  }

  ctx->backend = &g_long_double_backend; // default
  ctx->view.center_x = 0.0;
  ctx->view.center_y = 0.0;
  ctx->view.scale = 200.0; // zoom
  ctx->max_iterations = 100;
  ctx->render_strategy = RENDER_LINE_BY_LINE; // should remember selection (store in app?)
  ctx->fractal_type = FRACTAL_MANDELBROT; // should be setup accordingly to the app selection
  ctx->coloring_strategy = COLOR_GRAYSCALE; // should remember selection (store in app?)
  ctx->dirty = true;
  ctx->paused = false;

  return ctx;
}

void fractol_destroy_context(t_fractal_context *ctx) {
  if (!ctx) return;
  pixel_buffer_destroy(ctx->buffer);
  free(ctx);
}
