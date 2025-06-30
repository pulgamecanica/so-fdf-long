#include "state.h"
#include <stdlib.h>

/* each of these is defined in its .c */
#include "launcher_scene.h"
#include "game_menu_scene.h"
#include "gameplay_scene.h"

static const t_scene *current_scene = NULL;

void state_init(t_app *app)
{
  /* start at launcher */
  state_transition(app, STATE_LAUNCHER);
}

void state_transition(t_app *app, t_state new_state)
{
  if (current_scene && current_scene->on_exit)
    current_scene->on_exit(app);

  switch (new_state) {
    case STATE_LAUNCHER:
      current_scene = &launcher_scene;
      break;
    case STATE_GAME_MENU:
      current_scene = &game_menu_scene;
      break;
    case STATE_GAMEPLAY:
      current_scene = &gameplay_scene;
      break;
    case STATE_EXIT:
      exit(EXIT_SUCCESS);
  }

  if (current_scene->on_enter)
    current_scene->on_enter(app);
}

void state_update(t_app *app)
{
  if (current_scene && current_scene->update)
    current_scene->update(app);
}

void state_render(t_app *app)
{
  if (current_scene && current_scene->render)
    current_scene->render(app);
}

void state_restart(t_app *app) {
  if (!current_scene) return;

  if (current_scene->on_exit)
    current_scene->on_exit(app);
  if (current_scene->on_enter)
    current_scene->on_enter(app);
}