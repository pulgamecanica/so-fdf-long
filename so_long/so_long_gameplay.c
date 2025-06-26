#include "so_long/so_long_gameplay.h"
#include <stdio.h>

void so_long_gameplay_on_enter(t_app *app) {
  (void)app;
  printf("[So_Long] Entering gameplay scene\n");
}

void so_long_gameplay_on_exit(t_app *app) {
  (void)app;
  printf("[So_Long] Exiting gameplay scene\n");
}

void so_long_gameplay_update(t_app *app) {
  (void)app;
}

void so_long_gameplay_render(t_app *app) {
  (void)app;
}
