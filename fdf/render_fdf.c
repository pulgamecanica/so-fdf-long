#include "fdf/render_fdf.h"
#include "fdf/draw_utils.h"
#include <string.h>
#include <stdio.h>

void render_fdf(t_fdf *fdf) {
    if (!fdf || !fdf->canvas || !fdf->initialized)
        return;

    memset(fdf->canvas->pixels, 0, fdf->canvas->width * fdf->canvas->height * 4);

    for (int y = 0; y < fdf->map.height; ++y) {
        for (int x = 0; x < fdf->map.width; ++x) {
            t_point p = fdf->map.points[y][x];
            t_vec2i a;
            project_map_point(&fdf->cam, x, y, p.z, fdf->canvas->width, fdf->canvas->height, &a);
            uint32_t color_a = p.color;// >= 0 ? (uint32_t)p.color : 0xFFFFFFFF;

            if (x + 1 < fdf->map.width) {
                t_point pr = fdf->map.points[y][x + 1];
                t_vec2i b;
                project_map_point(&fdf->cam, x + 1, y, pr.z, fdf->canvas->width, fdf->canvas->height, &b);
                uint32_t color_b = pr.color;// >= 0 ? (uint32_t)pr.color : 0xFFFFFFFF;
                draw_line(fdf->canvas, a, b, color_a, color_b);
            }

            if (y + 1 < fdf->map.height) {
                t_point pd = fdf->map.points[y + 1][x];
                t_vec2i b;
                project_map_point(&fdf->cam, x, y + 1, pd.z, fdf->canvas->width, fdf->canvas->height, &b);
                uint32_t color_b = pd.color;// >= 0 ? (uint32_t)pd.color : 0xFFFFFFFF;
                draw_line(fdf->canvas, a, b, color_a, color_b);
            }
        }
    }
}
