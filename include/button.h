#ifndef BUTTON_H
#define BUTTON_H

#include <MLX42/MLX42.h>
#include <stdbool.h>
#include "vector2.h"

typedef enum e_button_state {
  BTN_IDLE,
  BTN_HOVER,
  BTN_ACTIVE
} t_button_state;

typedef struct s_button {
  t_vec2i position;
  t_vec2i size;
  t_button_state state;
  bool was_pressed;
  mlx_image_t* image;
  uint32_t color_idle;
  uint32_t color_hover;
  uint32_t color_active;
  void (*on_click)(void);
} t_button;

void button_init(t_button* btn, mlx_t* mlx, t_vec2i pos, t_vec2i size,
                 uint32_t color_idle, uint32_t color_hover, uint32_t color_active,
                 void (*on_click)(void));

void button_update(t_button* btn, int mouse_x, int mouse_y, bool is_down);
void button_render(t_button* btn);

#endif // BUTTON_H
