#include "fdf/fdf_gameplay.h"
#include "fdf/fdf.h"
#include "fdf/render_fdf.h"
#include "mlx_utils.h"
#include <stdio.h>

static t_fdf fdf;
static int error = 0;

void fdf_gameplay_on_enter(t_app *app) {
  const char *filename = fdf_get_map_filename(app->selected_map);
  if (!strlen(filename)) {
    fprintf(stderr, "[FDF] Initialization failed on file load.\n");
    error = 2;
    return;
  }
  if (!fdf_init(app, filename, &fdf)) {
      fprintf(stderr, "[FDF] Initialization failed.\n");
      error = 1;
      return;
  }
  error = 0;
  printf("[FDF] Entering gameplay scene\n");
}

void fdf_gameplay_on_exit(t_app *app) {
  (void)app;
  printf("[FDF] Exiting gameplay scene\n");
}

void fdf_gameplay_update(t_app *app) {
  (void)app;
  // You may want to allow key input to update fdf.cam here
}

void fdf_gameplay_render(t_app *app) {
  (void)app;
  if (error) return;
  
  render_fdf(&fdf);
}
