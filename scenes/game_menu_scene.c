#include "game_menu_scene.h"
#include "mlx_utils.h"
#include "state.h"
#include "fractol/fractol_menu.h"
#include "fdf/fdf_menu.h"
#include "so_long/so_long_menu.h"
#include <MLX42/MLX42.h>

#define MENU_COUNT  3

/* dispatch tables: index 0 = fractol, 1 = fdf, 2 = so_long */
static void (*const menu_on_enters[MENU_COUNT])(t_app *) = {
  fractol_menu_on_enter,
  fdf_menu_on_enter,
  so_long_menu_on_enter
};
static void (*const menu_on_exits[MENU_COUNT])(t_app *) = {
  fractol_menu_on_exit,
  fdf_menu_on_exit,
  so_long_menu_on_exit
};
static void (*const menu_updates[MENU_COUNT])(t_app *) = {
  fractol_menu_update,
  fdf_menu_update,
  so_long_menu_update
};
static void (*const menu_renders[MENU_COUNT])(t_app *) = {
  fractol_menu_render,
  fdf_menu_render,
  so_long_menu_render
};

static void on_enter(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= MENU_COUNT)
    return;
  menu_on_enters[idx](app);
}

static void on_exit(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= MENU_COUNT)
    return;
  menu_on_exits[idx](app);
}

static void update(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= MENU_COUNT)
    return;
  menu_updates[idx](app);
}

static void render(t_app *app)
{
  /* clear background */
  mlx_set_background(app->mlx, 0xaac1a1FF);

  int idx = app->selected_game;
  if (idx < 0 || idx >= MENU_COUNT)
    return;
  menu_renders[idx](app);
}

const t_scene game_menu_scene = {
  .on_enter = on_enter,
  .on_exit  = on_exit,
  .update   = update,
  .render   = render
};
