#include "launcher_scene.h"
#include "mlx_utils.h"
#include "state.h"
#include <MLX42/MLX42.h>

#define OPTIONS_COUNT 3

static int selection = 0;
// static const char *labels[OPTIONS_COUNT] = {
//   "fract-ol",
//   "fdf",
//   "so-long"
// };

static void on_enter(t_app *app)
{
  (void)app;
  selection = 0;
}

static void on_exit(t_app *app)
{
  (void)app;
  /* nothing to free yet */
}

static void update(t_app *app)
{
  /* move selection */
  if (mlx_is_key_down(app->mlx, MLX_KEY_UP))
    selection = (selection + OPTIONS_COUNT - 1) % OPTIONS_COUNT;
  else if (mlx_is_key_down(app->mlx, MLX_KEY_DOWN))
    selection = (selection + 1) % OPTIONS_COUNT;
  else if (mlx_is_key_down(app->mlx, MLX_KEY_ENTER)) {
    app->selected_game = selection;
    state_transition(app, STATE_GAME_MENU);
  }
}

static void render(t_app *app)
{
  /* clear to black */
  mlx_set_background(app->mlx, 0x4242FF);
  /* stub: you’d draw labels[0..2], highlighting ‘selection’ */
}

const t_scene launcher_scene = {
  .on_enter = on_enter,
  .on_exit  = on_exit,
  .update   = update,
  .render   = render
};
