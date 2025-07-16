#include "ui_manager.h"
#include "scroll_list.h"
#include "state.h"
#include "button.h"
#include "fdf/fdf_menu.h"
#include "mlx_utils.h"
#include <stdlib.h>
#include <string.h>

static const char *map_labels[] = {
  "Landscape 1",
  "Landscape 2",  
  "High Peaks",
  "Valley Bridge",
  "Crater Ridge",
  "42, life and everything",
  "42, element",
  "Mandelbrot",
  "Julia Set",
  "Mars Surface",
  "Pentenegpos",
  "Shape",
  "Twin Pyramids"
};

static const char *fdf_backgrounds[] = {
  "assets/fdf/Land1.png",
  "assets/fdf/Land2.png",  
  "assets/fdf/HighPeaks.png",
  "assets/fdf/Valley.png",
  "assets/fdf/Crater.png",
  "assets/fdf/42Life.png",
  "assets/fdf/42Element.png",
  "assets/fdf/Mandelbrot.png",
  "assets/fdf/JuliaSet.png",
  "assets/fdf/Mars.png",
  "assets/fdf/Pentenegpos.png",
  "assets/fdf/PyramidShape.png",
  "assets/fdf/PyramidTwin.png"
};


#define FDF_BG  "assets/fdf/bg.png"

#define MAP_COUNT (int)(sizeof(map_labels) / sizeof(*map_labels))
#define FDF_MENU_BUTTON_W 200
#define FDF_MENU_BUTTON_H 150
#define FDF_MENU_GAP 5

static t_ui_manager ui;
static t_scroll_list *scroll_list = NULL;
static t_button *map_buttons[MAP_COUNT];
static mlx_image_t *canvas = NULL;
static mlx_image_t *bg_image = NULL;

static void on_map_select(t_button *btn) {
  extern t_app g_app;

  for (int i = 0; i < MAP_COUNT; ++i) {
    if (btn == map_buttons[i]) {
      g_app.selected_map = i;
      state_transition(&g_app, STATE_GAMEPLAY);
      return;
    }
  }
}

static void init_bg(t_app *app) {
  if (bg_image) {
    mlx_delete_image(app->mlx, bg_image);
  };

  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;

  mlx_texture_t *bg_tex = mlx_load_png(FDF_BG);
  if (!bg_tex)
    exit(EXIT_FAILURE);

  bg_image = mlx_new_image(app->mlx, WIDTH, HEIGHT);
  if (!bg_image) {
    mlx_delete_texture(bg_tex);
    exit(EXIT_FAILURE);
  }

  // Simple nearest-neighbor resize
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      int src_x = x * bg_tex->width / WIDTH;
      int src_y = y * bg_tex->height / HEIGHT;
      uint32_t color = mlx_get_pixel_tex(bg_tex, src_x, src_y);
      
      // Decompose
      uint8_t r = (color >> 24) & 0xFF;
      uint8_t g = (color >> 16) & 0xFF;
      uint8_t b = (color >> 8)  & 0xFF;
      // Set alpha to 50% (128 out of 255)
      uint8_t a = 180;

      // Repack
      color = (r << 24) | (g << 16) | (b << 8) | a;
      
      my_mlx_put_pixel(bg_image, x, y, color);
    }
  }

  mlx_delete_texture(bg_tex);
}

void fdf_menu_on_enter(t_app *app) {
  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;
  const int LIST_H = HEIGHT - (HEIGHT / 5);

  int cx = (WIDTH - FDF_MENU_BUTTON_W) / 2;
  int cy = (HEIGHT - LIST_H) / 2;

  canvas = mlx_new_image(app->mlx, WIDTH, HEIGHT);
  if (!canvas) exit(EXIT_FAILURE);
  mlx_image_to_window(app->mlx, canvas, 0, 0);

  ui_manager_init(&ui);

  scroll_list = scroll_list_create(app->mlx, cx, cy,
                                    FDF_MENU_BUTTON_W,
                                    LIST_H,
                                    FDF_MENU_BUTTON_W,
                                    FDF_MENU_BUTTON_H,
                                    FDF_MENU_GAP);
  if (!scroll_list)
    exit(EXIT_FAILURE);

  for (int i = 0; i < MAP_COUNT; ++i) {
    map_buttons[i] = button_create(app->mlx, 0, 0,
                                   FDF_MENU_BUTTON_W,
                                   FDF_MENU_BUTTON_H,
                                   map_labels[i],
                                   on_map_select);
    if (strlen(fdf_backgrounds[i])) {
        button_set_background_path(map_buttons[i], fdf_backgrounds[i]);
    }
    if (scroll_list_add(scroll_list, button_as_element(map_buttons[i])) < 0)
        exit(EXIT_FAILURE);
  }


  ui_manager_add(&ui, scroll_list_as_element(scroll_list));
  init_bg(app);
}

void fdf_menu_on_exit(t_app *app) {
  for (int i = 0; i < MAP_COUNT; ++i)
    button_destroy(map_buttons[i]);
  ui_manager_remove(&ui, scroll_list_as_element(scroll_list), scroll_list_destroy);
  ui_manager_destroy(&ui);
  mlx_delete_image(app->mlx, canvas);
}

void fdf_menu_update(t_app *app) {
  ui_manager_update(&ui, app->mlx);

  // TODO: handle input & animations for FDF menu
}

void fdf_menu_render(t_app *app) {
  (void)app;
  if (bg_image) 
    put_img_to_img(canvas, bg_image, 0, 0);
  ui_manager_render(&ui, canvas);
  // TODO: draw FDF menu (buttons, labels, etc.)
}
