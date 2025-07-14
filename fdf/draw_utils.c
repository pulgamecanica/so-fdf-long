#include "mlx_utils.h"
#include "fdf/draw_utils.h"
#include <math.h>

uint32_t interpolate_color(uint32_t c1, uint32_t c2, float t) {
    uint8_t r1 = (c1 >> 16) & 0xFF;
    uint8_t g1 = (c1 >> 8) & 0xFF;
    uint8_t b1 = c1 & 0xFF;

    uint8_t r2 = (c2 >> 16) & 0xFF;
    uint8_t g2 = (c2 >> 8) & 0xFF;
    uint8_t b2 = c2 & 0xFF;

    uint8_t r = r1 + (int)((r2 - r1) * t);
    uint8_t g = g1 + (int)((g2 - g1) * t);
    uint8_t b = b1 + (int)((b2 - b1) * t);

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void project_map_point(const t_camera *cam, int x, int y, int z, int w, int h, t_vec2i *out) {
    int sx, sy;
    camera_project_point(x, y, z, cam, &sx, &sy);
    out->x = sx + w / 2;
    out->y = sy + h / 2;
}

void draw_line(mlx_image_t *img, t_vec2i a, t_vec2i b, uint32_t c1, uint32_t c2) {
    int dx = abs(b.x - a.x);
    int dy = -abs(b.y - a.y);
    int sx = (a.x < b.x) ? 1 : -1;
    int sy = (a.y < b.y) ? 1 : -1;
    int err = dx + dy;
    int x = a.x;
    int y = a.y;
    int steps = fmax(abs(b.x - a.x), abs(b.y - a.y));
    int i = 0;

    while (true) {
        float t = steps > 0 ? (float)i / (float)steps : 0.0f;
        uint32_t color = interpolate_color(c1, c2, t);

        if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
            my_mlx_put_pixel_alpha(img, x, y, color);

        if (x == b.x && y == b.y)
            break;

        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
        i++;
    }
}
