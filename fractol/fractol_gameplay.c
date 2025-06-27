#include "fractol/fractol_gameplay.h"
#include "fractol/dashboard.h"
#include "fractol/fractol.h"
#include "mlx_utils.h"
#include "state.h"
#include <stdio.h>

t_fractol *g_fractol = NULL;

void fractol_gameplay_on_enter(t_app *app) {
  (void)app;
  printf("[Fractol] Entering gameplay scene\n");

  g_fractol = fractol_create(app->mlx->width, app->mlx->height); // or use app->width/app->height
  if (!g_fractol) {
    fprintf(stderr, "Failed to initialize fractol\n");
    return;
  }
  init_fractol_dashboard(app, g_fractol);
}

void fractol_gameplay_on_exit(t_app *app) {
  printf("[Fractol] Exiting gameplay scene\n");
  (void)app;

  destroy_fractol_dashboard(app, g_fractol);
  fractol_destroy(g_fractol);
  g_fractol = NULL;
}

void fractol_gameplay_update(t_app *app) {
  (void)app;
  if (!g_fractol)
    return;

  if (mlx_is_key_down(app->mlx, MLX_KEY_P))
    g_fractol->paused = !g_fractol->paused;
  if (mlx_is_key_down(app->mlx, MLX_KEY_H))
    g_fractol->show_dashboard = !g_fractol->show_dashboard;

  fractol_update(g_fractol);

  if (g_fractol->show_dashboard)
    update_fractol_dashboard(app, g_fractol);
}

void fractol_gameplay_render(t_app *app) {
  if (!g_fractol)
  return;
  
  fractol_render(g_fractol);

  if (g_fractol->show_dashboard) {
    render_fractol_dashboard(app, g_fractol);
  } else if (g_fractol->paused) {
    mlx_put_string_to_image(app->mlx, g_fractol->canvas, "Paused", app->mlx->width / 2,  app->mlx->height / 2);
  }
}
