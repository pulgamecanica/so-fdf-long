#ifndef BUTTON_H
#define BUTTON_H

#include "ui_element.h"
#include <MLX42/MLX42.h>
#include <stdbool.h>

#define LETTER_PIXEL_SIZE_W 10
#define LETTER_PIXEL_SIZE_H 20
#define HELP_BUTTON_BG_PATH "assets/help.png"
#define CLOSE_BUTTON_BG_PATH "assets/close.png"

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

/**
 * Sets the background image from a file path (resized to button size).
 * Replaces any previous background.
 * Returns false on load or allocation failure.
 */
bool button_set_background_path(t_button *btn, const char *path);

/**
 * Sets the background color from a given color
 * Replaces any previous background.
 */
void button_set_background_color(t_button *btn, int color);

#endif // BUTTON_H
