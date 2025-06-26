#ifndef FRACTAL_CONTEXT_H
#define FRACTAL_CONTEXT_H

#include "pixel_buffer.h"
#include "fractol/numeric/backend.h"

typedef enum e_render_strategy {
  RENDER_LINE_BY_LINE,
  RENDER_SECTION,
  RENDER_CHESSBOARD
} t_render_strategy;

typedef enum e_coloring_strategy {
    COLOR_GRAYSCALE,
    COLOR_SMOOTH,
    COLOR_BANDS,
    COLOR_DISTANCE_ESTIMATE,
    COLOR_HSV,
    // Add more later
} t_coloring_strategy;

typedef enum e_fractal_type {
  FRACTAL_MANDELBROT,
  FRACTAL_MANDELBROT2,
  FRACTAL_JULIA,
  BURNING_SHIP,
  // Add more if needed
} t_fractal_type;

typedef struct s_viewport {
  double center_x;
  double center_y;
  double scale; // Pixels per unit
} t_viewport;

typedef struct s_fractal_context {
  const t_precision_backend *backend;
  t_pixel_buffer            *buffer;
  t_viewport                 view;
  int                        max_iterations;
  t_render_strategy          render_strategy;
  t_fractal_type             fractal_type;
  t_coloring_strategy        coloring_strategy;
  bool                       dirty;
  bool                       paused;
} t_fractal_context;

t_fractal_context *fractol_create_context(int width, int height);
void fractol_destroy_context(t_fractal_context *ctx);
void set_fractal_type(t_fractal_context *ctx, t_fractal_type type);

#endif
