#include "fractol/numeric/backend.h"
#include "fractol/fractal_context.h"
#include <stdlib.h>

typedef struct s_ldcoord {
  long double r;
  long double i;
} t_ldcoord;

static t_coord *ld_create(double real, double imag) {
  t_ldcoord *z = malloc(sizeof *z);
  if (!z) return NULL;
  z->r = (long double)real;
  z->i = (long double)imag;
  return (t_coord *)z;
}

static t_coord *ld_copy(const t_coord *src) {
  const t_ldcoord *z = (const t_ldcoord *)src;
  return ld_create((double)z->r, (double)z->i);
}

static void ld_destroy(t_coord *z) {
  free(z);
}

static void ld_set(t_coord *dst, const t_coord *src) {
  t_ldcoord *d = (t_ldcoord *)dst;
  const t_ldcoord *s = (const t_ldcoord *)src;
  d->r = s->r;
  d->i = s->i;
}

static void ld_square(t_coord *z) {
  t_ldcoord *c = (t_ldcoord *)z;
  long double r = c->r;
  long double i = c->i;
  c->r = r * r - i * i;
  c->i = 2.0 * r * i;
}

static void ld_add(t_coord *z, const t_coord *c) {
  t_ldcoord *a = (t_ldcoord *)z;
  const t_ldcoord *b = (const t_ldcoord *)c;
  a->r += b->r;
  a->i += b->i;
}

static double ld_modulus_squared(const t_coord *z) {
  const t_ldcoord *c = (const t_ldcoord *)z;
  return (double)(c->r * c->r + c->i * c->i);
}

static void ld_screen_to_world(t_fractal_context *ctx, int x, int y, t_coord *out) {
  t_ldcoord *coord = (t_ldcoord *)out;

  const double scale = ctx->view.scale;
  coord->r = ctx->view.center_x + (x - ctx->buffer->width / 2.0) / scale;
  coord->i = ctx->view.center_y + (y - ctx->buffer->height / 2.0) / scale;
}

static uint32_t ld_compute_pixel(t_fractal_context *ctx, const t_coord *z_) {
  const t_ldcoord *c = (const t_ldcoord *)z_;
  t_ldcoord z = { 0.0L, 0.0L };

  uint32_t iter = 0;
  while (iter < (uint32_t)ctx->max_iterations && (z.r * z.r + z.i * z.i) <= 4.0L) {
    long double tmp = z.r * z.r - z.i * z.i + c->r;
    z.i = 2.0L * z.r * z.i + c->i;
    z.r = tmp;
    iter++;
  }
  return iter;
}

const t_precision_backend g_long_double_backend = {
  .name = "long double",
  .create = ld_create,
  .copy = ld_copy,
  .destroy = ld_destroy,
  .set = ld_set,
  .square = ld_square,
  .add = ld_add,
  .modulus_squared = ld_modulus_squared,
  .screen_to_world = ld_screen_to_world,
  .compute_pixel = ld_compute_pixel
};
