#include "app.h"
#include "launcher_scene.h"
#include "scene.h"
#include <MLX42/MLX42.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WEB
# include <emscripten.h>
# include <emscripten/html5.h>
#endif

#define WIDTH 800
#define HEIGHT 600

t_app g_app;
t_app* g_app_ptr = &g_app;


#ifdef WEB
void emscripten_main_loop(void) {
  mlx_loop(g_app.mlx);
}
#endif

static void scene_update_wrapper(void* param) {
  scene_update((t_app*)param);
}

void launcher_start(void) {
  g_app.mlx = mlx_init(WIDTH, HEIGHT, "MLX42 Launcher", true);
  if (!g_app.mlx) {
    puts(mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
  }

  set_scene(g_launcher_scene, &g_app);
  mlx_loop_hook(g_app.mlx, scene_update_wrapper, &g_app);

#ifdef WEB
  emscripten_set_main_loop(emscripten_main_loop, 0, true);
#else
  mlx_loop(g_app.mlx);
#endif

  mlx_terminate(g_app.mlx);
}
