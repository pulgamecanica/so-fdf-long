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
# include "ui_manager.h"
# include "scroll_list.h"

typedef struct s_fdf {
    t_app           *app;
    t_map           map;
    t_camera        cam;
    bool            initialized;
    const char      *filename;
    mlx_image_t     *canvas;
    bool            dragging;
    int             drag_start_x;
    int             drag_start_y;
    float           cam_start_offset_x;
    float           cam_start_offset_y;
    float           cam_start_offset_z;
    t_ui_manager    *ui;
    t_scroll_list   *proj_selection;
} t_fdf;

int fdf_init(t_app *app, const char *filename, t_fdf *fdf);
void fdf_deinit(t_fdf *fdf);

#endif
