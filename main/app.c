#include "app.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WEB
# include <emscripten.h>
# include <emscripten/html5.h>
#endif

/* user-adjustable window size */
#define WIDTH  800
#define HEIGHT 600

static t_app g_app;

/* called every frame by mlx_loop_hook */
static void loop_hook(void *param)
{
  t_app *app = param;

  state_update(app);
  state_render(app);
}

#ifdef WEB
static void emscripten_main_loop(void)
{
  mlx_loop(g_app.mlx);
}
#endif

void start(void)
{
  mlx_set_setting(MLX_MAXIMIZED, true);
  g_app.mlx = mlx_init(WIDTH, HEIGHT, "MLX42 Launcher", true);
  if (!g_app.mlx) {
    fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
  }

  state_init(&g_app);
  mlx_loop_hook(g_app.mlx, loop_hook, &g_app);

#ifdef WEB
  emscripten_set_main_loop(emscripten_main_loop, 0, true);
#else
  mlx_loop(g_app.mlx);
#endif

  mlx_terminate(g_app.mlx);
}
