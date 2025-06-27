#include "fractol/color.h"
#include <math.h>

static uint32_t grayscale(uint32_t iter, uint32_t max_iter) {
    if (iter >= max_iter) return 0x000000FF;
    uint8_t v = (uint8_t)(255.0 * (float)iter / max_iter);
    return (v << 24) | (v << 16) | (v << 8) | 0xFF;
}

static uint32_t smooth(uint32_t iter, uint32_t max_iter) {
    if (iter >= max_iter) return 0x000000FF;
    float t = (float)iter / (float)max_iter;
    float smoothed = t * t * (3.0f - 2.0f * t); // smoothstep
    uint8_t c = (uint8_t)(smoothed * 255.0f);
    return (c << 24) | (c << 16) | (255 - c) << 8 | 0xFF;
}

static uint32_t bands(uint32_t iter, uint32_t max_iter) {
    if (iter >= max_iter) return 0x000000FF;

    uint32_t band_count = 6;
    uint32_t band = (iter * band_count) / max_iter;

    switch (band) {
        case 0: return 0x0000FFFF; // Blue
        case 1: return 0x00FFFFFF; // Cyan
        case 2: return 0x00FF00FF; // Green
        case 3: return 0xFFFF00FF; // Yellow
        case 4: return 0xFF8000FF; // Orange
        case 5: return 0xFF0000FF; // Red
        default: return 0xFFFFFFFF; // White fallback
    }
}

static uint32_t distance(uint32_t iter, uint32_t max_iter, const t_coord *z, const t_fractal_context *ctx) {
    if (iter >= max_iter) return 0x000000FF;

    double mod2 = ctx->backend->modulus_squared(z);
    double log_zn = log(mod2) / 2.0;
    double nu = log(log_zn / log(2.0)) / log(2.0);
    double smooth_iter = iter + 1.0 - nu;

    float t = (float)(smooth_iter / max_iter);
    uint8_t v = (uint8_t)(255.0f * t);

    return (v << 24) | (v << 16) | (v << 8) | 0xFF;
}

static uint32_t hsv(uint32_t iter, uint32_t max_iter, const t_coord *z, const t_fractal_context *ctx) {
    if (iter >= max_iter) return 0x000000FF;

    float t = (float)iter / (float)max_iter;

    // Optional: use z’s modulus to vary hue subtly
    double mod_sq = ctx->backend->modulus_squared(z);
    float hue = fmodf(t + (float)mod_sq * 0.05f, 1.0f);
    float s = 1.0f;
    float v = 1.0f;

    int i = (int)(hue * 6.0f);
    float f = hue * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float r = v * (1.0f - (1.0f - f) * s);

    float red, green, blue;
    switch (i % 6) {
        case 0: red = v; green = r; blue = p; break;
        case 1: red = q; green = v; blue = p; break;
        case 2: red = p; green = v; blue = r; break;
        case 3: red = p; green = q; blue = v; break;
        case 4: red = r; green = p; blue = v; break;
        case 5: red = v; green = p; blue = q; break;
        default: red = v; green = p; blue = q;;
    }

    return ((uint8_t)(red * 255) << 24) |
           ((uint8_t)(green * 255) << 16) |
           ((uint8_t)(blue * 255) << 8) | 0xFF;
}

uint32_t iter_to_color(uint32_t iter, uint32_t max_iter, const t_coord *z, const t_fractal_context *ctx) {
// uint32_t iter_to_color(uint32_t iter, uint32_t max_iter, const t_fractal_context *ctx) {
    switch (ctx->coloring_strategy) {
        case COLOR_GRAYSCALE: return grayscale(iter, max_iter);
        case COLOR_SMOOTH: return smooth(iter, max_iter);
        case COLOR_BANDS: return bands(iter, max_iter);
        case COLOR_DISTANCE_ESTIMATE: return distance(iter, max_iter, z, ctx);
        case COLOR_HSV: return hsv(iter, max_iter, z, ctx);
        // Add more here as implemented
        default: return 0xFFFFFFFF; // fallback white
    }
}
