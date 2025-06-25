#include "launcher_scene.h"
#include "state.h"
#include "ui_manager.h"
#include "button.h"
#include "mlx_utils.h"
#include "state.h"
#include <MLX42/MLX42.h>
#include <stdlib.h>

#define OPTIONS_COUNT 3
#define BUTTON_WIDTH  200
#define BUTTON_HEIGHT 50
#define SPACING       20


static t_ui_manager  ui;
static t_button     *buttons[OPTIONS_COUNT];
static mlx_image_t  *canvas;

static int selection = 0;
static const char *labels[OPTIONS_COUNT] = {
  "fract-ol",
  "fdf",
  "so-long"
};

static void btn_callback(t_button *btn)
{
  extern t_app g_app;

  for (int i = 0; i < OPTIONS_COUNT; ++i) {
    if (buttons[i] == btn) {
      g_app.selected_game = i;
      state_transition(&g_app, STATE_GAME_MENU);
      return;
    }
  }
}

static void add_buttons(t_app* app) {
  /* compute vertical centering */
  int total_h = OPTIONS_COUNT * BUTTON_HEIGHT
              + (OPTIONS_COUNT - 1) * SPACING;
  int start_y = (app->mlx->height - total_h) / 2;

  /* create & register buttons */
  for (int i = 0; i < OPTIONS_COUNT; ++i) {
    int bx = (app->mlx->width  - BUTTON_WIDTH) / 2;
    int by = start_y + i * (BUTTON_HEIGHT + SPACING);
    buttons[i] = button_create(app->mlx,
                               bx, by,
                               BUTTON_WIDTH, BUTTON_HEIGHT,
                               labels[i],
                               btn_callback);
    ui_manager_add(&ui, button_as_element(buttons[i]));
  }
}


static void on_scene_enter(t_app *app)
{
  selection = 0;
  canvas = mlx_new_image(app->mlx, app->mlx->width, app->mlx->height);
  if (!canvas)
    exit(EXIT_FAILURE);
  mlx_set_background(app->mlx, 0x4242FF);
  mlx_image_to_window(app->mlx, canvas, 0, 0);
  ui_manager_init(&ui);

  add_buttons(app);
}

static void on_scene_exit(t_app *app)
{
  for (int i = 0; i < OPTIONS_COUNT; ++i) {
    ui_manager_remove(&ui,
                      button_as_element(buttons[i]),
                      button_destroy);
  }
  ui_manager_destroy(&ui);

  mlx_delete_image(app->mlx, canvas);
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

  ui_manager_update(&ui, app->mlx);
}

static void render(t_app *app)
{
  /* clear to black */
  mlx_set_background(app->mlx, 0x4242FF);
  /* stub: you’d draw labels[0..2], highlighting ‘selection’ */
  ui_manager_render(&ui, canvas);
}

const t_scene launcher_scene = {
  .on_enter = on_scene_enter,
  .on_exit  = on_scene_exit,
  .update   = update,
  .render   = render
};
