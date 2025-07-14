#include "fdf/fdf.h"
#include <string.h>
#include <stdio.h>
#include <button.h>

static t_button *map_buttons[2];
extern t_fdf fdf;
static t_ui_manager ui;

static void on_proj_select(t_button *btn) {
  for (int i = 0; i < 2; ++i)
    if (btn == map_buttons[i])
      fdf.cam.mode = (t_projection_mode)(i);
}

static void init_proj_selection(t_app *app, t_fdf *fdf_) {
    const int PADDING = 10;
    const int PROJ_W = 100;
    const int PROJ_H = 100;

    ui_manager_init(&ui);
    fdf_->ui = &ui;

    fdf_->proj_selection = scroll_list_create(app->mlx, PADDING, PADDING,
                                    PROJ_W,
                                    PROJ_H * 2.5,
                                    PROJ_W,
                                    PROJ_H,
                                    0);
    if (!fdf_->proj_selection)
        exit(EXIT_FAILURE);

    map_buttons[0] = button_create(app->mlx, 0, 0, PROJ_W, PROJ_W, "ISO", on_proj_select);
    map_buttons[1] = button_create(app->mlx, 0, 0, PROJ_W, PROJ_W, "ORTHO", on_proj_select);
    if (scroll_list_add(fdf_->proj_selection, button_as_element(map_buttons[0])) < 0)
      exit(EXIT_FAILURE);
    if (scroll_list_add(fdf_->proj_selection, button_as_element(map_buttons[1])) < 0)
      exit(EXIT_FAILURE);

  ui_manager_add(&ui, scroll_list_as_element(fdf_->proj_selection));
}

int fdf_init(t_app *app, const char *filename, t_fdf *fdf_) {
    if (!app || !filename || !fdf_)
        return 0;

    if (fdf_->initialized && fdf_->filename && strcmp(fdf_->filename, filename) == 0)
        return 1;

    if (!fdf_->initialized)
        init_proj_selection(app, fdf_);

    fdf_deinit(fdf_);
    
    fdf_->app = app;
    camera_init(&fdf_->cam);


    if (map_load(filename, &fdf_->map) != 0) {
        fprintf(stderr, "[FDF] Failed to load map: %s\n", filename);
        return 0;
    }

    fdf_->canvas = mlx_new_image(app->mlx, app->mlx->width, app->mlx->height);
    if (!fdf_->canvas) {
        fprintf(stderr, "[FDF] Failed to allocate canvas\n");
        map_free(&fdf_->map);
        return 0;
    }

    if (mlx_image_to_window(app->mlx, fdf_->canvas, 0, 0) < 0) {
        fprintf(stderr, "[FDF] Failed to add image to window\n");
        mlx_delete_image(app->mlx, fdf_->canvas);
        fdf_->canvas = NULL;
        map_free(&fdf_->map);
        return 0;
    }

    fdf_->filename = filename;
    fdf_->initialized = true;
    return 1;
}

void fdf_deinit(t_fdf *fdf_) {
    if (!fdf_ || !fdf_->initialized)
        return;
    map_free(&fdf_->map);
    if (fdf_->canvas)
        mlx_delete_image(fdf_->app->mlx, fdf_->canvas);
    fdf_->canvas = NULL;
    fdf_->initialized = false;
    fdf_->filename = NULL;
}