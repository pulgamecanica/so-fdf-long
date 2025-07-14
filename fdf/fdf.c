#include "fdf/fdf.h"
#include <string.h>
#include <stdio.h>

int fdf_init(t_app *app, const char *filename, t_fdf *fdf) {
    if (!app || !filename || !fdf)
        return 0;

    if (fdf->initialized && fdf->filename && strcmp(fdf->filename, filename) == 0)
        return 1;

    fdf_deinit(fdf);
    
    fdf->app = app;
    camera_init(&fdf->cam);

    if (map_load(filename, &fdf->map) != 0) {
        fprintf(stderr, "[FDF] Failed to load map: %s\n", filename);
        return 0;
    }

    fdf->canvas = mlx_new_image(app->mlx, app->mlx->width, app->mlx->height);
    if (!fdf->canvas) {
        fprintf(stderr, "[FDF] Failed to allocate canvas\n");
        map_free(&fdf->map);
        return 0;
    }

    if (mlx_image_to_window(app->mlx, fdf->canvas, 0, 0) < 0) {
        fprintf(stderr, "[FDF] Failed to add image to window\n");
        mlx_delete_image(app->mlx, fdf->canvas);
        fdf->canvas = NULL;
        map_free(&fdf->map);
        return 0;
    }

    fdf->filename = filename;
    fdf->initialized = true;
    return 1;
}

void fdf_deinit(t_fdf *fdf) {
    if (!fdf || !fdf->initialized)
        return;
    map_free(&fdf->map);
    if (fdf->canvas)
        mlx_delete_image(fdf->app->mlx, fdf->canvas);
    fdf->canvas = NULL;
    fdf->initialized = false;
    fdf->filename = NULL;
}