#include "fractol/render/strategy.h"
#include "fractol/numeric/backend.h"
#include <stdio.h>

void render_line_by_line(t_fractal_context *ctx) {
  t_pixel_buffer *buf = ctx->buffer;
  const t_precision_backend *b = ctx->backend;

  t_coord *real = b->create(0.0, 0.0);
  if (!real) return; // handle failure once
  for (int y = 0; y < buf->height; ++y) {
    for (int x = 0; x < buf->width; ++x) {
      int idx = y * buf->width + x;
      if (buf->data[idx].done)
        continue;

      // t_coord *real = b->create(0.0, 0.0); 
      // if (!real) continue;

      b->screen_to_world(ctx, x, y, real);
      uint32_t iter = b->compute_pixel(ctx, real);

      // b->destroy(real);

      buf->data[idx].iterations = iter;
      buf->data[idx].done = true;
    }
  }
  b->destroy(real);
  ctx->dirty = false;
}

