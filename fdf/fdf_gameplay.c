#include "fdf/fdf_gameplay.h"
#include "fdf/fdf.h"
#include "fdf/render_fdf.h"
#include "mlx_utils.h"
#include <stdio.h>

#define PAN_SPEED   10.0f

t_fdf fdf;
static int error = 0;

void fdf_gameplay_on_enter(t_app *app) {
  const char *filename = fdf_get_map_filename(app->selected_map);
  if (!strlen(filename)) {
    fprintf(stderr, "[FDF] Initialization failed on file load.\n");
    error = 2;
    return;
  }
  if (!fdf_init(app, filename, &fdf)) {
      fprintf(stderr, "[FDF] Initialization failed.\n");
      error = 1;
      return;
  }
  error = 0;
  printf("[FDF] Entering gameplay scene\n");
}

void fdf_gameplay_on_exit(t_app *app) {
  (void)app;
  printf("[FDF] Exiting gameplay scene\n");
}

void fdf_gameplay_update(t_app *app) {
  if (!fdf.initialized)
    return;

  bool shift_pressed =  mlx_is_key_down(app->mlx, MLX_KEY_LEFT_SHIFT) ||  mlx_is_key_down(app->mlx, MLX_KEY_RIGHT_SHIFT);
  if (app->scroll_y > 0 || (mlx_is_key_down(app->mlx, MLX_KEY_EQUAL) && shift_pressed))
    fdf.cam.zoom *= 1.05f;
  if (app->scroll_y < 0 || mlx_is_key_down(app->mlx, MLX_KEY_MINUS))
    fdf.cam.zoom *= 0.95f;

  // --- Keyboard movement ---
  const float rot_speed = PAN_SPEED * 0.001; 
  if (mlx_is_key_down(app->mlx, MLX_KEY_A))
    fdf.cam.rot_offset_x -= rot_speed;
  if (mlx_is_key_down(app->mlx, MLX_KEY_D))
    fdf.cam.rot_offset_x += rot_speed;
  if (mlx_is_key_down(app->mlx, MLX_KEY_W))
    fdf.cam.rot_offset_y -= rot_speed;
  if (mlx_is_key_down(app->mlx, MLX_KEY_S))
    fdf.cam.rot_offset_y += rot_speed;
  if (mlx_is_key_down(app->mlx, MLX_KEY_Q))
    fdf.cam.rot_offset_z += rot_speed;
  if (mlx_is_key_down(app->mlx, MLX_KEY_E))
    fdf.cam.rot_offset_z -= rot_speed;


  if (mlx_is_key_down(app->mlx, MLX_KEY_LEFT))
    fdf.cam.offset_x -= PAN_SPEED;
  if (mlx_is_key_down(app->mlx, MLX_KEY_RIGHT))
    fdf.cam.offset_x += PAN_SPEED;
  if (mlx_is_key_down(app->mlx, MLX_KEY_UP))
    fdf.cam.offset_y -= PAN_SPEED;
  if (mlx_is_key_down(app->mlx, MLX_KEY_DOWN))
    fdf.cam.offset_y += PAN_SPEED;

  // --- Mouse dragging ---
  if (mlx_is_mouse_down(app->mlx, MLX_MOUSE_BUTTON_LEFT)) {
    int mx, my;
    mlx_get_mouse_pos(app->mlx, &mx, &my);

    bool ctrl_pressed = mlx_is_key_down(app->mlx, MLX_KEY_LEFT_CONTROL) || mlx_is_key_down(app->mlx, MLX_KEY_RIGHT_CONTROL);

    if (!fdf.dragging) {
      fdf.dragging = true;
      fdf.drag_start_x = mx;
      fdf.drag_start_y = my;
      fdf.cam_start_offset_x = fdf.cam.offset_x;
      fdf.cam_start_offset_y = fdf.cam.offset_y;
      fdf.cam_start_offset_z = fdf.cam.offset_z;
    } else {
      float dx = (float)(mx - fdf.drag_start_x);
      float dy = (float)(my - fdf.drag_start_y);

      if (ctrl_pressed) {
        fdf.cam.offset_z = fmaxf(0.01f, fdf.cam_start_offset_z + dy * 0.01f);  // sensitivity control
      } else {
        fdf.cam.offset_x = fdf.cam_start_offset_x + dx;
        fdf.cam.offset_y = fdf.cam_start_offset_y + dy;
      }
    }
  } else {
    fdf.dragging = false;
  }
  ui_manager_update(fdf.ui, app->mlx);
}

void fdf_gameplay_render(t_app *app) {
  (void)app;
  if (error) return;
  
  render_fdf(&fdf);
  ui_manager_render(fdf.ui, fdf.canvas);
}
