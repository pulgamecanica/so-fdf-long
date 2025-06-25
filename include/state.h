#ifndef STATE_H
#define STATE_H

#include "app.h"
#include "scene.h"

/*
 * all possible screens
 */
typedef enum e_state {
  STATE_LAUNCHER,
  STATE_GAME_MENU,
  STATE_GAMEPLAY,
  STATE_EXIT,
} t_state;

/* core state-machine API */
void state_init(t_app *app);
void state_transition(t_app *app, t_state new_state);
void state_update(t_app *app);
void state_render(t_app *app);

#endif // STATE_H
