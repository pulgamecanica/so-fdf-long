#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <MLX42/MLX42.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward */
typedef struct s_ui_element t_ui_element;

/* Common callback types */
typedef void (*t_ui_update)(t_ui_element *self, mlx_t *mlx);
typedef void (*t_ui_render)(t_ui_element *self, mlx_image_t *canvas);
typedef void (*t_ui_on_click)(t_ui_element *self, int button, int x, int y);

/* Base UI element */
struct s_ui_element {
  int                x, y;         /* top-left */
  int                w, h;         /* size */
  bool               visible;
  t_ui_update        update;
  t_ui_render        render;
  t_ui_on_click      on_click;
  void              *userdata;     /* for extending types */
  /* you can add layer/z-order here if needed */
};

/* Helper to test point inside element */
static inline bool ui_element_hit(t_ui_element *e, int mx, int my) {
  return e->visible
      && mx >= e->x && mx < e->x + e->w
      && my >= e->y && my < e->y + e->h;
}

#endif // UI_ELEMENT_H
