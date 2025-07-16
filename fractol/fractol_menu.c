#include "fractol/fractol_menu.h"
#include "ui_manager.h"
#include "scroll_list.h"
#include "state.h"
#include "mlx_utils.h"
#include "button.h"
#include <stdlib.h>
#include <string.h>

static const char *fractal_labels[] = {
  "Mandelbrot",
  "Mandelbrot2.0",
  "JuliaSet",
  "Burning Ship",
};

static const char *fractal_backgrounds[] = {
  "assets/fractol/mandelbrot.png", 
  "assets/fractol/mandelbrot2.png",  
  "assets/fractol/julia_set.png",
  "assets/fractol/burning_ship.png"
};

#define FARCTOL_MAP_COUNT (int)(sizeof(fractal_labels) / sizeof(*fractal_labels))
#define FRACTOL_MENU_BUTTON_W 140
#define FRACTOL_MENU_BUTTON_H 100
#define FRACTOL_MENU_GAP 5
#define FRACTOL_LIST_H  340
#define FRACTOL_BG  "assets/fractol/bg.png"

static t_ui_manager ui;
static t_scroll_list *scroll_list = NULL;
static t_button *fractal_buttons[FARCTOL_MAP_COUNT];
static mlx_image_t *canvas = NULL;
static mlx_image_t *bg_image = NULL;

static void on_map_select(t_button *btn) {
  extern t_app g_app;

  for (int i = 0; i < FARCTOL_MAP_COUNT; ++i) {
    if (btn == fractal_buttons[i]) {
      g_app.selected_map = i;
      state_transition(&g_app, STATE_GAMEPLAY);
      return;
    }
  }
}

static void init_bg(t_app *app) {
  if (bg_image) return;

  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;

  mlx_texture_t *bg_tex = mlx_load_png(FRACTOL_BG);
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

void fractol_menu_on_enter(t_app *app) {
  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;

  int cx = (WIDTH - FRACTOL_MENU_BUTTON_W) / 2;
  int cy = (HEIGHT - FRACTOL_LIST_H) / 3;

  canvas = mlx_new_image(app->mlx, WIDTH, HEIGHT);
  if (!canvas) exit(EXIT_FAILURE);
  mlx_image_to_window(app->mlx, canvas, 0, 0);

  ui_manager_init(&ui);

  scroll_list = scroll_list_create(app->mlx, cx, cy,
                                    FRACTOL_MENU_BUTTON_W,
                                    app->mlx->height - 20,
                                    FRACTOL_MENU_BUTTON_W,
                                    FRACTOL_MENU_BUTTON_H,
                                    FRACTOL_MENU_GAP);
  if (!scroll_list)
    exit(EXIT_FAILURE);

  for (int i = 0; i < FARCTOL_MAP_COUNT; ++i) {
    fractal_buttons[i] = button_create(app->mlx, 0, 0, FRACTOL_MENU_BUTTON_W, FRACTOL_MENU_BUTTON_H, fractal_labels[i], on_map_select);
    if (strlen(fractal_backgrounds[i]))
      button_set_background_path(fractal_buttons[i], fractal_backgrounds[i]);
    if (scroll_list_add(scroll_list, button_as_element(fractal_buttons[i])) < 0)
      exit(EXIT_FAILURE);
  }

  ui_manager_add(&ui, scroll_list_as_element(scroll_list));
  init_bg(app);
}

void fractol_menu_on_exit(t_app *app) {
  for (int i = 0; i < FARCTOL_MAP_COUNT; ++i)
    button_destroy(fractal_buttons[i]);
  ui_manager_remove(&ui, scroll_list_as_element(scroll_list), scroll_list_destroy);
  ui_manager_destroy(&ui);
  mlx_delete_image(app->mlx, bg_image);
  mlx_delete_image(app->mlx, canvas);
  bg_image = NULL;
}

void fractol_menu_update(t_app *app)
{
  ui_manager_update(&ui, app->mlx);
}

void fractol_menu_render(t_app *app)
{
  (void)app;
  if (bg_image) 
    put_img_to_img(canvas, bg_image, 0, 0);
  ui_manager_render(&ui, canvas);
}
