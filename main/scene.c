#include "scene.h"

static t_scene g_current_scene = {0};

void set_scene(t_scene new_scene, t_app* app) {
  if (g_current_scene.destroy)
    g_current_scene.destroy(app);
  g_current_scene = new_scene;
  if (g_current_scene.init)
    g_current_scene.init(app);
}

void scene_update(t_app* app) {
  if (g_current_scene.update)
    g_current_scene.update(app);
}
