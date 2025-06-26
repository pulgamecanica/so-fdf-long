#include "fractol/fractol_gameplay.h"
#include <stdio.h>

void fractol_gameplay_on_enter(t_app *app) {
  (void)app;
  printf("[Fractol] Entering gameplay scene\n");
}

void fractol_gameplay_on_exit(t_app *app) {
  (void)app;
  printf("[Fractol] Exiting gameplay scene\n");
}

void fractol_gameplay_update(t_app *app) {
  (void)app;
}

void fractol_gameplay_render(t_app *app) {
  (void)app;
}
