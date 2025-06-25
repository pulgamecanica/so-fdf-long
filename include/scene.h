#ifndef SCENE_H
#define SCENE_H

#include "app.h"

/*
 * each scene implements these four
 */
typedef struct s_scene {
  void (*on_enter)(t_app*);
  void (*on_exit)(t_app*);
  void (*update)(t_app*);  /* poll input & advance animations */
  void (*render)(t_app*);  /* draw frame */
} t_scene;


#endif // SCENE_H
