#include "fractol/numeric/fixed_point.h"
#include "fractol/numeric/backend.h"
#include <stdlib.h>

typedef struct s_fpcoord {
  fixed_t r;
  fixed_t i;
} t_fpcoord;

fixed_t fixed_from_double(double d) {
  return (fixed_t)(d * (double)FIXED_ONE);
}

double fixed_to_double(fixed_t f) {
  return (double)f / (double)FIXED_ONE;
}

fixed_t fixed_mul(fixed_t a, fixed_t b) {
  return (a * b) >> FIXED_FRAC_BITS;
}

fixed_t fixed_square(fixed_t a) {
  return fixed_mul(a, a);
}

static t_coord *fp_create(double real, double imag) {
  t_fpcoord *z = malloc(sizeof *z);
  if (!z) return NULL;
  z->r = fixed_from_double(real);
  z->i = fixed_from_double(imag);
  return (t_coord *)z;
}

static t_coord *fp_copy(const t_coord *src) {
  const t_fpcoord *z = (const t_fpcoord *)src;
  return fp_create(fixed_to_double(z->r), fixed_to_double(z->i));
}

static void fp_destroy(t_coord *z) {
  free(z);
}

static void fp_set(t_coord *dst, const t_coord *src) {
  t_fpcoord *d = (t_fpcoord *)dst;
  const t_fpcoord *s = (const t_fpcoord *)src;
  d->r = s->r;
  d->i = s->i;
}

static void fp_square(t_coord *z) {
  t_fpcoord *c = (t_fpcoord *)z;
  fixed_t r = c->r;
  fixed_t i = c->i;
  c->r = fixed_square(r) - fixed_square(i);
  c->i = fixed_mul(r, i) * 2;
}

static void fp_add(t_coord *z, const t_coord *c) {
  t_fpcoord *a = (t_fpcoord *)z;
  const t_fpcoord *b = (const t_fpcoord *)c;
  a->r += b->r;
  a->i += b->i;
}

static double fp_modulus_squared(const t_coord *z) {
  const t_fpcoord *c = (const t_fpcoord *)z;
  fixed_t mod_sq = fixed_square(c->r) + fixed_square(c->i);
  return fixed_to_double(mod_sq);
}

static void fp_screen_to_world(t_fractal_context *ctx, int x, int y, t_coord *out) {
  (void)ctx;(void)x;(void)y;(void)out;
  
}

static double fp_get_re(const t_coord *z) {
  return fixed_to_double(((t_fpcoord *)z)->r);
}

static double fp_get_im(const t_coord *z) {
  return fixed_to_double(((t_fpcoord *)z)->i);
}

static void fp_set_re(t_coord *z, double re) {
  ((t_fpcoord *)z)->r = fixed_from_double(re);
}

static void fp_set_im(t_coord *z, double im) {
  ((t_fpcoord *)z)->i = fixed_from_double(im);
}


const t_precision_backend g_fixed_point_backend = {
  .name = "fixed point 32.32",
  .create = fp_create,
  .copy = fp_copy,
  .destroy = fp_destroy,
  .set = fp_set,
  .square = fp_square,
  .add = fp_add,
  .modulus_squared = fp_modulus_squared,
  .screen_to_world = fp_screen_to_world,
  .get_re = fp_get_re,
  .get_im = fp_get_im,
  .set_re = fp_set_re,
  .set_im = fp_set_im
};
