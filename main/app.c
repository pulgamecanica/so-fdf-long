#include "app.h"
#include "state.h"
#include "mlx_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WEB
# include <emscripten.h>
# include <emscripten/html5.h>
#endif

/* user-adjustable window size */
#define WIDTH 1200
#define HEIGHT 800
#define RESIZE_WINDOW_PATH "assets/resize_window_bg.png"

t_app g_app;

static mlx_image_t *resize_image = NULL;
static mlx_texture_t *resize_tex = NULL;

static void init_resize_image(t_app *app) {
  util_resize_texture_to_image(app, &resize_tex, RESIZE_WINDOW_PATH, &resize_image);
}

/* called every frame by mlx_loop_hook */
static void loop_hook(void *param)
{
  t_app *app = param;

  if (!app->size_ok) {
    mlx_image_to_window(app->mlx, resize_image, 0, 0);
    return ;
  }

  if (app->should_resize) {
    state_restart(app);
    app->should_resize = false;
  }

  state_update(app);
  state_render(app);

  app->scroll_x = 0.0;
  app->scroll_y = 0.0;
}

static void scroll_callback(double xdelta, double ydelta, void *param)
{
  t_app *app = param;
  
  app->scroll_x += xdelta;
  app->scroll_y += ydelta;
}

static void resize_hook(int32_t width, int32_t height, void* param) {
  t_app *app = param;

  if (width < 650 || height < 400)
    app->size_ok = false;
  else
    app->size_ok = true;
  app->should_resize = true;
  init_resize_image(app);
}


#ifdef WEB
static void emscripten_main_loop(void)
{
  mlx_loop(g_app.mlx);
}
#endif

void start(void)
{
  // mlx_set_setting(MLX_MAXIMIZED, true);
  g_app.mlx = mlx_init(WIDTH, HEIGHT, "MLX42 so-fdf-long", true);
  g_app.size_ok = true;
  g_app.should_resize = false;
  if (!g_app.mlx) {
    fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
  }

  state_init(&g_app);
  mlx_scroll_hook(g_app.mlx, scroll_callback, &g_app);
  mlx_loop_hook(g_app.mlx, loop_hook, &g_app);
  mlx_resize_hook(g_app.mlx, resize_hook, &g_app);

#ifdef WEB
  emscripten_set_main_loop(emscripten_main_loop, 0, true);
#else
  mlx_loop(g_app.mlx);
#endif

  if (resize_image)
    mlx_delete_image(g_app.mlx, resize_image);
  if (resize_tex)
    mlx_delete_texture(resize_tex);
  mlx_terminate(g_app.mlx);
}
