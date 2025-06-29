#include "fractol/render/strategy.h"
#include "fractol/numeric/backend.h"
#include "fractol/formula.h"
#include <stdio.h>

extern t_fractal_formula g_fractal_formulas[];



#include <time.h>

static double get_time_seconds() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void render_line_by_line(t_fractal_context *ctx) {
  if (!ctx || !ctx->buffer || !ctx->backend) return;

  t_pixel_buffer *buf = ctx->buffer;
  const t_precision_backend *b = ctx->backend;
  t_coord *real = b->create(0.0, 0.0);
  if (!real) return;

  const int max_calls = 10000;
  const double max_time = 0.04; // 40 milliseconds

  int calls = 0;
  double start_time = get_time_seconds();

  for (int y = ctx->cursor_y; y < buf->height; ++y) {
    for (int x = (y == ctx->cursor_y ? ctx->cursor_x : 0); x < buf->width; ++x) {
      int idx = y * buf->width + x;
      if (buf->data[idx].done)
        continue;

      b->screen_to_world(ctx, x, y, real);
      uint32_t iter = g_fractal_formulas[ctx->fractal_type](real, ctx);
      buf->data[idx].iterations = iter;
      buf->data[idx].done = true;

      calls++;
      if (calls >= max_calls || (get_time_seconds() - start_time) >= max_time) {
        ctx->cursor_x = x;
        ctx->cursor_y = y;
        b->destroy(real);
        return;
      }
    }
  }

  // All done
  ctx->cursor_x = 0;
  ctx->cursor_y = 0;
  ctx->dirty = false;

  b->destroy(real);
}
