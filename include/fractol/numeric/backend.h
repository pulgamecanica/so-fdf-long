#ifndef FRACTOL_BACKEND_H
#define FRACTOL_BACKEND_H

#include <stdbool.h>
#include <stdint.h>

/* Forward-declared opaque type */
typedef struct s_coord t_coord;
typedef struct s_fractal_context t_fractal_context;

// struct s_coord {
//   long double re;
//   long double im;
// };

/* Backend interface */
typedef struct s_precision_backend {
  const char *name;

  t_coord * (*create)(double real, double imag);
  t_coord * (*copy)(const t_coord *src);
  void      (*destroy)(t_coord *z);

  void      (*set)(t_coord *dst, const t_coord *src);
  void      (*square)(t_coord *z);
  void      (*add)(t_coord *z, const t_coord *c);
  double    (*modulus_squared)(const t_coord *z);

  void      (*screen_to_world)(t_fractal_context *ctx, int x, int y, t_coord *out);
  uint32_t  (*compute_pixel)(t_fractal_context *ctx, const t_coord *z);
} t_precision_backend;

#endif
