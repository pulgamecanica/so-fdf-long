#include "ui_manager.h"
#include <stdlib.h>

void ui_manager_init(t_ui_manager *mgr)
{
  mgr->elements = NULL;
  mgr->count    = 0;
}

void ui_manager_destroy(t_ui_manager *mgr)
{
  free(mgr->elements);
  mgr->elements = NULL;
  mgr->count    = 0;
}

void ui_manager_add(t_ui_manager *mgr, t_ui_element *e)
{
  mgr->elements = realloc(mgr->elements,
                          sizeof *mgr->elements * (mgr->count + 1));
  mgr->elements[mgr->count++] = e;
}

void ui_manager_remove(t_ui_manager *mgr, 
                       t_ui_element   *e,
                       void          (*free_fn)(void*))
{
  size_t i, j = 0;
  for (i = 0; i < mgr->count; ++i) {
    if (mgr->elements[i] == e) {
      /* call destructor on the user data */
      if (free_fn)
        free_fn(e->userdata);
      continue;
    }
    mgr->elements[j++] = mgr->elements[i];
  }
  mgr->count = j;
  mgr->elements = realloc(mgr->elements,
                          sizeof *mgr->elements * mgr->count);
}

void ui_manager_update(t_ui_manager *mgr, mlx_t *mlx)
{
  int mx;
  int my;
  mlx_get_mouse_pos(mlx, &mx, &my);

  /* 1) per-element update() */
  for (size_t i = 0; i < mgr->count; ++i)
    if (mgr->elements[i]->update)
      mgr->elements[i]->update(mgr->elements[i], mlx);

  /* 2) check clicks */
  if (mlx_is_mouse_down(mlx, MLX_MOUSE_BUTTON_LEFT)) {
    for (size_t i = 0; i < mgr->count; ++i) {
      t_ui_element *e = mgr->elements[i];
      if (e->on_click && ui_element_hit(e, mx, my))
        e->on_click(e, MLX_MOUSE_BUTTON_LEFT, mx, my);
    }
  }
}

void ui_manager_render(t_ui_manager *mgr, mlx_image_t *canvas)
{
  for (size_t i = 0; i < mgr->count; ++i)
    if (mgr->elements[i]->render)
      mgr->elements[i]->render(mgr->elements[i], canvas);
}
