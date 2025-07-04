#include "fractol/fractal_context.h"
#include "app.h"
#include <stdlib.h>
#include <string.h>

t_fractal_context *fractol_create_context(int width, int height) {
  extern t_precision_backend g_long_double_backend;
  extern t_app g_app;

  t_fractal_context *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  memset(ctx, 0, sizeof *ctx);
  ctx->buffer = pixel_buffer_create(width, height);
  if (!ctx->buffer) {
    free(ctx);
    return NULL;
  }

  ctx->backend = &g_long_double_backend;
  ctx->view.center_x = 0.0;
  ctx->view.center_y = 0.0;
  ctx->view.scale = 200.0; // zoom
  ctx->max_iterations = 100;
  ctx->render_strategy = RENDER_LINE_BY_LINE; // should remember selection (store in app?)
  ctx->coloring_strategy = COLOR_GRAYSCALE; // should remember selection (store in app?)
  ctx->dirty = true;
  ctx->paused = false;
  ctx->cursor_x = 0;
  ctx->cursor_y = 0;
  ctx->force_redraw = true;
  ctx->max_formula_calls = 10000;
  ctx->max_time_per_frame = 0.004;

  set_fractal_type(ctx, g_app.selected_map);
  return ctx;
}

void set_fractal_type(t_fractal_context *ctx, t_fractal_type type) {
  ctx->fractal_type = type;
}

void fractol_destroy_context(t_fractal_context *ctx) {
  if (!ctx) return;
  pixel_buffer_destroy(ctx->buffer);
  free(ctx);
}

void fractol_reset_render(t_fractal_context *ctx) {
  if (!ctx || !ctx->buffer) return;

  ctx->cursor_x = 0;
  ctx->cursor_y = 0;
  ctx->dirty = true;
  ctx->force_redraw = false;

  for (int i = 0; i < ctx->buffer->width * ctx->buffer->height; ++i) {
    ctx->buffer->data[i].done = false;
    ctx->buffer->data[i].iterations = 0;
  }
}
