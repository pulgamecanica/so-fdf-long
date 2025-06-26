#include "fractol/fractol_gameplay.h"
#include "fractol/fractol.h"
#include "mlx_utils.h"
#include "state.h"
#include <stdio.h>

static t_fractol *g_fractol = NULL;

void fractol_gameplay_on_enter(t_app *app) {
  (void)app;
  printf("[Fractol] Entering gameplay scene\n");

  g_fractol = fractol_create(app->mlx->width, app->mlx->height); // or use app->width/app->height
  if (!g_fractol) {
    fprintf(stderr, "Failed to initialize fractol\n");
    return;
  }
}

void fractol_gameplay_on_exit(t_app *app) {
  printf("[Fractol] Exiting gameplay scene\n");
  (void)app;

  fractol_destroy(g_fractol);
  g_fractol = NULL;
}

void fractol_gameplay_update(t_app *app) {
  (void)app;
  if (!g_fractol)
    return;

  if (mlx_is_key_down(app->mlx, MLX_KEY_P))
    g_fractol->paused = !g_fractol->paused;

  fractol_update(g_fractol);
}

void fractol_gameplay_render(t_app *app) {
  (void)app;
  if (!g_fractol)
    return;
  fractol_render(g_fractol);
}
