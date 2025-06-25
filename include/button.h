#ifndef BUTTON_H
#define BUTTON_H

#include "ui_element.h"
#include <MLX42/MLX42.h>
#include <stdbool.h>

/* Opaque button type */
typedef struct s_button t_button;

/**
 * Create a new rectangular button.
 * @param mlx      MLX instance (for creating image/textures)
 * @param x,y,w,h  position and size
 * @param label    text to draw inside
 * @param cb       user callback on click
 * @return pointer you must free with button_destroy()
 */
t_button *button_create(mlx_t *mlx,
                        int x, int y, int w, int h,
                        const char *label,
                        void (*cb)(t_button*));

/**
 * Destroy a button.
 * 
 * NOTE: signature must match `void (*free_fn)(void*)`
 */
void button_destroy(void *userdata);

/* access the UI element to register in manager */
t_ui_element *button_as_element(t_button *btn);

#endif // BUTTON_H
