#include "fractol/render/strategy.h"

/* forward declarations */
void render_line_by_line(t_fractal_context *ctx);
void render_section(t_fractal_context *ctx);
void render_chessboard(t_fractal_context *ctx);

t_render_fn g_render_strategies[] = {
  render_line_by_line,
  render_section,
  render_chessboard
};
