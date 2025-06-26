
/* scroll_list.h */
#ifndef SCROLL_LIST_H
#define SCROLL_LIST_H

#include "ui_element.h"
#include <MLX42/MLX42.h>
#include <stddef.h>

/**
 * Vertical scroll list UI element.
 * Items must be UI elements of identical size.
 */
typedef struct s_scroll_list t_scroll_list;

/**
 * Create a vertical scrolling container.
 * @param mlx      MLX instance
 * @param x, y     Top-left position
 * @param w, h     Container dimensions
 * @param item_w   Width of each item (must match elem->w)
 * @param item_h   Height of each item (must match elem->h)
 * @return new scroll list (NULL on alloc failure)
 */
t_scroll_list *scroll_list_create(mlx_t *mlx,
                                  int x, int y,
                                  int w, int h,
                                  int item_w, int item_h, int gap);

/**
 * Destroy scroll list and free its container.
 * Pass this to ui_manager_remove() as the free_fn.
 */
void scroll_list_destroy(void *userdata);

/**
 * Add a UI element (e.g. button) as an item.
 * @return 0 on success, -1 on error (size mismatch)
 */
int scroll_list_add(t_scroll_list *list,
                    t_ui_element  *elem);

/**
 * Access base UI element for registration.
 */
t_ui_element *scroll_list_as_element(t_scroll_list *list);

#endif // SCROLL_LIST_H