#ifndef FDF_H
# define FDF_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <string.h>

# include "map.h"
# include "app.h"
# include "camera.h"

typedef struct s_fdf {
    t_app       *app;
    t_map       map;
    t_camera    cam;
    bool        initialized;
    const char  *filename;
    mlx_image_t *canvas;
} t_fdf;

int fdf_init(t_app *app, const char *filename, t_fdf *fdf);
void fdf_deinit(t_fdf *fdf);

#endif
