#include "gameplay_scene.h"
#include "mlx_utils.h"
#include "state.h"
#include <MLX42/MLX42.h>
#include <stdio.h>

static void on_enter(t_app *app)
{
  (void)app;
  puts("Entered gameplay");
  /* load textures/shaders for app->selected_game/app->selected_map */
}

static void on_exit(t_app *app)
{
  (void)app;
  /* free resources */
}

static void update(t_app *app)
{
  if (mlx_is_key_down(app->mlx, MLX_KEY_ESCAPE))
    state_transition(app, STATE_GAME_MENU);

  /* stub: run fract-ol, fdf or so-long logic */
}

static void render(t_app *app)
{
  mlx_set_background(app->mlx, 0x222222FF);
  /* stub: draw your game frame */
}

const t_scene gameplay_scene = {
  .on_enter = on_enter,
  .on_exit  = on_exit,
  .update   = update,
  .render   = render
};
