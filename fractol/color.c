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

// TODO: implement other modes

uint32_t iter_to_color(uint32_t iter, uint32_t max_iter, const t_fractal_context *ctx) {
    switch (ctx->coloring_strategy) {
        case COLOR_GRAYSCALE: return grayscale(iter, max_iter);
        case COLOR_SMOOTH: return smooth(iter, max_iter);
        // Add more here as implemented
        default: return 0xFFFFFFFF; // fallback white
    }
}
