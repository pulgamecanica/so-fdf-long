#include "gameplay_scene.h"
#include "mlx_utils.h"
#include "state.h"
#include "fractol/fractol_gameplay.h"
#include "fdf/fdf_gameplay.h"
#include "so_long/so_long_gameplay.h"
#include <MLX42/MLX42.h>

#define GAME_COUNT  3

/* dispatch tables */
static void (*const gameplay_on_enters[GAME_COUNT])(t_app *) = {
  fractol_gameplay_on_enter,
  fdf_gameplay_on_enter,
  so_long_gameplay_on_enter
};
static void (*const gameplay_on_exits[GAME_COUNT])(t_app *) = {
  fractol_gameplay_on_exit,
  fdf_gameplay_on_exit,
  so_long_gameplay_on_exit
};
static void (*const gameplay_updates[GAME_COUNT])(t_app *) = {
  fractol_gameplay_update,
  fdf_gameplay_update,
  so_long_gameplay_update
};
static void (*const gameplay_renders[GAME_COUNT])(t_app *) = {
  fractol_gameplay_render,
  fdf_gameplay_render,
  so_long_gameplay_render
};

static void on_enter(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= GAME_COUNT)
    return;
  gameplay_on_enters[idx](app);
}

static void on_exit(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= GAME_COUNT)
    return;
  gameplay_on_exits[idx](app);
}

static void update(t_app *app)
{
  int idx = app->selected_game;
  if (idx < 0 || idx >= GAME_COUNT)
    return;
  gameplay_updates[idx](app);

  if (mlx_is_key_down(app->mlx, MLX_KEY_ESCAPE))
    state_transition(app, STATE_GAME_MENU);
}

static void render(t_app *app)
{
  mlx_set_background(app->mlx, 0x222222FF);

  int idx = app->selected_game;
  if (idx < 0 || idx >= GAME_COUNT)
    return;
  gameplay_renders[idx](app);
}

const t_scene gameplay_scene = {
  .on_enter = on_enter,
  .on_exit  = on_exit,
  .update   = update,
  .render   = render
};
