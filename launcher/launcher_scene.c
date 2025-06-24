#include "launcher_scene.h"
#include "button.h"
#include "so_long_scene.h"
#include "scene.h"
#include "launcher_bg.h"

// static t_button btn;

// static void on_click_so_long(void) {
//   extern t_app* g_app_ptr;
//   set_scene(g_so_long_scene, g_app_ptr);
// }

void launcher_init(t_app* app) {
  launcher_bg_init(app);
}

void launcher_update(t_app* app) {
  launcher_bg_update(app);
}

void launcher_destroy(t_app* app) {
  launcher_bg_destroy(app);
}

t_scene g_launcher_scene = {
  .init = launcher_init,
  .update = launcher_update,
  .destroy = launcher_destroy
};
