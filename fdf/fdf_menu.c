#include "ui_manager.h"
#include "scroll_list.h"
#include "state.h"
#include "button.h"
#include "fdf/fdf_menu.h"
#include <stdlib.h>

static const char *map_labels[] = {
    "High Peaks (100-6)",
    "Valley Bridge (10-2)",
    "Rolling Hills (10-70)",
    "Crater Ridge (20-60)",
    "42, life and everything",
    "Jagged Terrain (50-4)",
    "Basic Terrain Test",
    "Elemental Columns",
    "Colored Elements",
    "Element Grid",
    "Fractal Element",
    "Julia Landscape",
    "Mars Surface",
    "Negative Plateaus",
    "Flatland Plateau",
    "Flat Puzzle (PnP)",
    "Power Tower (Pylone)",
    "Pyramid Shape",
    "Twin Pyramids",
    "Test Landscape 1",
    "Test Landscape 2"
};

#define MAP_COUNT (int)(sizeof(map_labels) / sizeof(*map_labels))
#define FDF_MENU_BUTTON_W 250
#define FDF_MENU_BUTTON_H 40
#define FDF_MENU_GAP 5

static t_ui_manager ui;
static t_scroll_list *scroll_list = NULL;
static t_button *map_buttons[MAP_COUNT];
static mlx_image_t *canvas = NULL;

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
    map_buttons[i] = button_create(app->mlx, 0, 0, FDF_MENU_BUTTON_W, FDF_MENU_BUTTON_H, map_labels[i], on_map_select);
    if (scroll_list_add(scroll_list, button_as_element(map_buttons[i])) < 0)
      exit(EXIT_FAILURE);
  }

  ui_manager_add(&ui, scroll_list_as_element(scroll_list));
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
  ui_manager_render(&ui, canvas);
  // TODO: draw FDF menu (buttons, labels, etc.)
}
