#include "so_long_scene.h"
#include <stdio.h>

void so_long_init(t_app* app) {
  (void)app;
  printf("So_Long Scene Initialized\n");
}

void so_long_update(t_app* app) {
  (void)app;
  // future logic
}

void so_long_destroy(t_app* app) {
  (void)app;
  // cleanup
}

t_scene g_so_long_scene = {
  .init = so_long_init,
  .update = so_long_update,
  .destroy = so_long_destroy
};
