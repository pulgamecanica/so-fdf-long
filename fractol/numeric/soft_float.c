#include "fractol/numeric/backend.h"
#include "fractol/fractal_context.h"
#include <stdlib.h>

typedef struct s_sfcoord {
  double r;
  double i;
} t_sfcoord;

static t_coord *sf_create(double real, double imag) {
  t_sfcoord *z = malloc(sizeof *z);
  if (!z) return NULL;
  z->r = real;
  z->i = imag;
  return (t_coord *)z;
}

static t_coord *sf_copy(const t_coord *src) {
  const t_sfcoord *z = (const t_sfcoord *)src;
  return sf_create(z->r, z->i);
}

static void sf_destroy(t_coord *z) {
  free(z);
}

static void sf_set(t_coord *dst, const t_coord *src) {
  t_sfcoord *d = (t_sfcoord *)dst;
  const t_sfcoord *s = (const t_sfcoord *)src;
  d->r = s->r;
  d->i = s->i;
}

static void sf_square(t_coord *z) {
  t_sfcoord *c = (t_sfcoord *)z;
  double r = c->r;
  double i = c->i;
  c->r = r * r - i * i;
  c->i = 2.0 * r * i;
}

static void sf_add(t_coord *z, const t_coord *c) {
  t_sfcoord *a = (t_sfcoord *)z;
  const t_sfcoord *b = (const t_sfcoord *)c;
  a->r += b->r;
  a->i += b->i;
}

static double sf_modulus_squared(const t_coord *z) {
  const t_sfcoord *c = (const t_sfcoord *)z;
  return c->r * c->r + c->i * c->i;
}

static void sf_screen_to_world(t_fractal_context *ctx, int x, int y, t_coord *out) {
  (void)ctx;(void)x;(void)y;(void)out;
}

static double sf_get_re(const t_coord *z) {
  return ((t_sfcoord *)z)->r;
}

static double sf_get_im(const t_coord *z) {
  return ((t_sfcoord *)z)->i;
}

static void sf_set_re(t_coord *z, double re) {
  ((t_sfcoord *)z)->r = (float)re;
}

static void sf_set_im(t_coord *z, double im) {
  ((t_sfcoord *)z)->i = (float)im;
}

const t_precision_backend g_soft_float_backend = {
  .name = "soft float",
  .create = sf_create,
  .copy = sf_copy,
  .destroy = sf_destroy,
  .set = sf_set,
  .square = sf_square,
  .add = sf_add,
  .modulus_squared = sf_modulus_squared,
  .screen_to_world = sf_screen_to_world,
  .get_re = sf_get_re,
  .get_im = sf_get_im,
  .set_re = sf_set_re,
  .set_im = sf_set_im
};
