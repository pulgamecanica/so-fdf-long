#include "fdf/fdf_gameplay.h"
#include <stdio.h>

void fdf_gameplay_on_enter(t_app *app) {
  (void)app;
  printf("[FDF] Entering gameplay scene\n");
}

void fdf_gameplay_on_exit(t_app *app) {
  (void)app;
  printf("[FDF] Exiting gameplay scene\n");
}

void fdf_gameplay_update(t_app *app) {
  (void)app;
}

void fdf_gameplay_render(t_app *app) {
  (void)app;
}
