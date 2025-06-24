#ifndef SCENE_H
#define SCENE_H

#include "app.h"

typedef struct s_scene {
  void (*init)(t_app*);
  void (*update)(t_app*);
  void (*destroy)(t_app*);
} t_scene;

void set_scene(t_scene new_scene, t_app* app);
void scene_update(t_app* app);

#endif
