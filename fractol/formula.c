#include "fractol/fractal_context.h"
#include "fractol/formula.h"
#include "fractol/numeric/backend.h"
#include <math.h>

static uint32_t formula_mandelbrot(const t_coord *c, t_fractal_context *ctx) {
  const t_precision_backend *b = ctx->backend;
  t_coord *z = b->create(0.0L, 0.0L);

  uint32_t iter = 0;
  while (iter < (uint32_t)ctx->max_iterations && b->modulus_squared(z) <= 4.0) {
    b->square(z);    // z = z²
    b->add(z, c);    // z = z + c
    iter++;
  }

  b->destroy(z);
  return iter;
}

static uint32_t formula_mandelbrot2(const t_coord *c, t_fractal_context *ctx) {
  const t_precision_backend *b = ctx->backend;
  t_coord *z = b->create(0.0L, 0.0L);

  uint32_t iter = 0;
  while (iter < (uint32_t)ctx->max_iterations && b->modulus_squared(z) <= 4.0) {
    // z = z⁴ + c
    b->square(z);    // z = z²
    b->square(z);    // z = z²
    b->add(z, c);    // z = z + c
    iter++;
  }

  b->destroy(z);
  return iter;
}

static uint32_t formula_julia(const t_coord *z0, t_fractal_context *ctx) {
  const t_precision_backend *b = ctx->backend;

  // Expose these values via the context or UI
  double cre = -0.8;
  double cim = 0.156;

  t_coord *c = b->create(cre, cim);
  t_coord *z = b->copy(z0);

  uint32_t iter = 0;
  while (iter < (uint32_t)ctx->max_iterations && b->modulus_squared(z) <= 4.0) {
    b->square(z);
    b->add(z, c);
    iter++;
  }

  b->destroy(c);
  b->destroy(z);
  return iter;
}


static uint32_t formula_burning_ship(const t_coord *c, t_fractal_context *ctx) {
  const t_precision_backend *b = ctx->backend;
  t_coord *z = b->create(0.0, 0.0);

  uint32_t iter = 0;
  while (iter < (uint32_t)ctx->max_iterations && b->modulus_squared(z) <= 4.0) {
    double zr = fabs(b->get_re(z));
    double zi = fabs(b->get_im(z));
    b->set_re(z, zr);
    b->set_im(z, zi);

    b->square(z);
    b->add(z, c);
    iter++;
  }

  b->destroy(z);
  return iter;
}


t_fractal_formula g_fractal_formulas[] = {
  formula_mandelbrot,
  formula_mandelbrot2,
  formula_julia,
  formula_burning_ship
};
