#include "game_menu_scene.h"
#include "mlx_utils.h"
#include "state.h"
#include <MLX42/MLX42.h>

#define ITEMS_PER_GAME  5  /* stub */
static int selection = 0;

static void on_enter(t_app *app)
{
  (void)app;
  selection = 0;
}

static void on_exit(t_app *app)
{
  (void)app;

}

static void update(t_app *app)
{
  if (mlx_is_key_down(app->mlx, MLX_KEY_ESCAPE)) {
    state_transition(app, STATE_LAUNCHER);
    return;
  }
  if (mlx_is_key_down(app->mlx, MLX_KEY_UP))
    selection = (selection + ITEMS_PER_GAME - 1) % ITEMS_PER_GAME;
  else if (mlx_is_key_down(app->mlx, MLX_KEY_DOWN))
    selection = (selection + 1) % ITEMS_PER_GAME;
  else if (mlx_is_key_down(app->mlx, MLX_KEY_ENTER)) {
    app->selected_map = selection;
    state_transition(app, STATE_GAMEPLAY);
  }
}

static void render(t_app *app)
{
  mlx_set_background(app->mlx, 0xff1111FF);
  /* stub: draw map/function list for app->selected_game */
}

const t_scene game_menu_scene = {
  .on_enter = on_enter,
  .on_exit  = on_exit,
  .update   = update,
  .render   = render
};
