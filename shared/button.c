#include "button.h"
#include <stdlib.h>

// Internal: fill image with solid color
static void fill_rect(mlx_image_t* img, t_vec2i size, uint32_t color) {
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      mlx_put_pixel(img, x, y, color);
    }
  }
}

// Init button and render initial idle state
void button_init(t_button* btn, mlx_t* mlx, t_vec2i pos, t_vec2i size,
                 uint32_t color_idle, uint32_t color_hover, uint32_t color_active,
                 void (*on_click)(void)) {
  btn->position = pos;
  btn->size = size;
  btn->state = BTN_IDLE;
  btn->was_pressed = false;
  btn->color_idle = color_idle;
  btn->color_hover = color_hover;
  btn->color_active = color_active;
  btn->on_click = on_click;

  btn->image = mlx_new_image(mlx, size.x, size.y);
  if (!btn->image)
    exit(EXIT_FAILURE);
  if (mlx_image_to_window(mlx, btn->image, pos.x, pos.y) < 0)
    exit(EXIT_FAILURE);

  fill_rect(btn->image, size, color_idle);
}

// Render based on current visual state
void button_render(t_button* btn) {
  uint32_t color = btn->color_idle;
  if (btn->state == BTN_HOVER)
    color = btn->color_hover;
  else if (btn->state == BTN_ACTIVE)
    color = btn->color_active;
  fill_rect(btn->image, btn->size, color);
}

// Update state and handle click
void button_update(t_button* btn, int mouse_x, int mouse_y, bool is_down) {
  bool inside = (mouse_x >= btn->position.x && mouse_x < btn->position.x + btn->size.x &&
                 mouse_y >= btn->position.y && mouse_y < btn->position.y + btn->size.y);

  if (inside) {
    if (is_down) {
      btn->state = BTN_ACTIVE;
      btn->was_pressed = true;
    } else {
      if (btn->was_pressed && btn->on_click)
        btn->on_click();
      btn->state = BTN_HOVER;
      btn->was_pressed = false;
    }
  } else {
    btn->state = BTN_IDLE;
    btn->was_pressed = false;
  }
}
