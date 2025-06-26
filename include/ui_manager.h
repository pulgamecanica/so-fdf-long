#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ui_element.h"
#include <MLX42/MLX42.h>
#include <stddef.h>

typedef struct s_ui_manager {
  t_ui_element **elements;
  size_t         count;
  bool           was_mouse_down;
} t_ui_manager;

/* Initialize/tear-down; you must call destroy when done */
void        ui_manager_init(t_ui_manager *mgr);
void        ui_manager_destroy(t_ui_manager *mgr);

/* Register/unregister an element */
void        ui_manager_add   (t_ui_manager *mgr, t_ui_element *e);
void        ui_manager_remove(t_ui_manager *mgr, 
                              t_ui_element   *e,
                              void          (*free_fn)(void* userdata));

/* Poll input & update all, then handle mouse clicks */
void        ui_manager_update(t_ui_manager *mgr, mlx_t *mlx);
/* Render in insertion order (you can change to sorted by layer) */
void        ui_manager_render(t_ui_manager *mgr, mlx_image_t *canvas);

#endif // UI_MANAGER_H
