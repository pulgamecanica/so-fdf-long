#ifndef DRAW_UTILS_H
# define DRAW_UTILS_H

# include "fdf.h"
# include <stdint.h>

typedef struct s_vec2i {
    int x;
    int y;
} t_vec2i;

uint32_t interpolate_color(uint32_t c1, uint32_t c2, float t);
void project_map_point(const t_camera *cam, int x, int y, int z, int w, int h, t_vec2i *out);
void draw_line(mlx_image_t *img, t_vec2i a, t_vec2i b, uint32_t c1, uint32_t c2);

#endif
