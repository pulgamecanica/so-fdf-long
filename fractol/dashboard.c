#include "fractol/dashboard.h"
#include "scroll_list.h"
#include "ui_manager.h"
#include "mlx_utils.h"
#include "button.h"
#include <stdlib.h>
#include <stdio.h>

static const char *color_labels[] = {
  "grayscale",
  "smooth",
  "bands",
  "distance",
  "hsv",
};


#define FRACTOL_DASHBOARD_WIDTH 500
#define FRACTOL_DASHBOARD_HEIGTH 300
#define COLOR_STRATEGY_COUNT 5
#define COLOR_STRATEGY_COUNT 5
#define COLOR_BUTTON_W 150
#define COLOR_BUTTON_H 20
#define COLOR_BUTTON_GAP 5
#define CLOSE_BUTTON_SIZE 16

static t_ui_manager ui;
static t_scroll_list *color_scroll_list = NULL;
static t_button *color_buttons[COLOR_STRATEGY_COUNT];
static t_button *close_dashboard;
extern t_fractol *g_fractol;


static void on_map_select(t_button *btn) {
  extern t_app g_app;


  for (int i = 0; i < COLOR_STRATEGY_COUNT; ++i) {
    if (btn == color_buttons[i]) {
      g_app.selected_map = i;
      g_fractol->ctx->coloring_strategy = i;
      return;
    }
  }
}

static void on_close_dashboard(t_button *btn)
{
  (void)btn;
  if (g_fractol)
    g_fractol->show_dashboard = false;
}

void init_fractol_dashboard(t_app *app, const t_fractol *f) {
  (void)f;
  ui_manager_init(&ui);

  const int dashboard_padding = 30;

  const int dashboard_x = (app->mlx->width - FRACTOL_DASHBOARD_WIDTH) / 2;
  const int dashboard_y = (app->mlx->height - FRACTOL_DASHBOARD_HEIGTH) / 2;

  int cx = (app->mlx->width / 2) - (FRACTOL_DASHBOARD_WIDTH / 2) + dashboard_padding;
  int cy = (app->mlx->height / 2) - (FRACTOL_DASHBOARD_HEIGTH / 2) + dashboard_padding;

  color_scroll_list = scroll_list_create(app->mlx, cx, cy,
                                    COLOR_BUTTON_W,
                                    COLOR_BUTTON_H * 4,
                                    COLOR_BUTTON_W,
                                    COLOR_BUTTON_H,
                                    COLOR_BUTTON_GAP);
  if (!color_scroll_list)
    exit(EXIT_FAILURE);

  for (int i = 0; i < COLOR_STRATEGY_COUNT; ++i) {
    color_buttons[i] = button_create(app->mlx, 0, 0, COLOR_BUTTON_W, COLOR_BUTTON_H, color_labels[i], on_map_select);
    if (scroll_list_add(color_scroll_list, button_as_element(color_buttons[i])) < 0)
      exit(EXIT_FAILURE);
  }

  close_dashboard = button_create(
    app->mlx,
    dashboard_x + FRACTOL_DASHBOARD_WIDTH - CLOSE_BUTTON_SIZE - 10, 
    dashboard_y + 10,
    CLOSE_BUTTON_SIZE,
    CLOSE_BUTTON_SIZE,
    "",
    on_close_dashboard
  );

  button_set_background_path(close_dashboard, CLOSE_BUTTON_BG_PATH);
  button_set_background_color(close_dashboard, 0x424242FF);
  ui_manager_add(&ui, button_as_element(close_dashboard));
  ui_manager_add(&ui, scroll_list_as_element(color_scroll_list));
}

void update_fractol_dashboard(t_app *app, const t_fractol *f) {
  // key bindings
  (void)f;
  ui_manager_update(&ui, app->mlx);
}

void render_fractol_dashboard(t_app *app, const t_fractol *f) {
  if (!f || !f->canvas)
    return;
  const int x_start = app->mlx->width / 2 - FRACTOL_DASHBOARD_WIDTH / 2;
  const int y_start = app->mlx->height / 2 - FRACTOL_DASHBOARD_HEIGTH / 2;
  
  const int border = 1;

  for (int y = 0; y < FRACTOL_DASHBOARD_HEIGTH + border * 2; ++y) {
    for (int x = 0; x < FRACTOL_DASHBOARD_WIDTH + border * 2; ++x) {
      my_mlx_put_pixel(f->canvas, x_start - border + x, y_start - border + y, 0x0311bfFF);
    }
  }
  
  for (int y = 0; y < FRACTOL_DASHBOARD_HEIGTH; ++y) {
    for (int x = 0; x < FRACTOL_DASHBOARD_WIDTH; ++x) {
      my_mlx_put_pixel(f->canvas, x_start + x, y_start + y, 0xFF0000);
    }
  }

  // mlx_image_to_window(app->mlx, f->canvas, 0, 0);
  ui_manager_render(&ui, f->canvas);


  {
    int cx = (app->mlx->width / 2) - (FRACTOL_DASHBOARD_WIDTH / 2) + COLOR_BUTTON_W * 2;
    int cy = (app->mlx->height / 2) - (FRACTOL_DASHBOARD_HEIGTH / 2) + COLOR_BUTTON_H * 3;

    mlx_put_string_to_image(app->mlx, f->canvas, color_labels[f->ctx->coloring_strategy], cx, cy);
  }
}

void destroy_fractol_dashboard(t_app *app, const t_fractol *f) {
  (void)app;
  (void)f;
  button_destroy(close_dashboard);
  for (int i = 0; i < COLOR_STRATEGY_COUNT; ++i)
    button_destroy(color_buttons[i]);
  ui_manager_remove(&ui, scroll_list_as_element(color_scroll_list), scroll_list_destroy);
  ui_manager_destroy(&ui);

}