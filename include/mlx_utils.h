// background.h
#ifndef MLX_UTILS_H
#define MLX_UTILS_H

#include <MLX42/MLX42.h>
#include <stdint.h>

/**
 * Clear the entire window to the given RGBA color.
 *
 * @param mlx   The MLX42 instance (holds the GL context).
 * @param color 0xRRGGBBAA packed color.
 */
void mlx_set_background(mlx_t *mlx, uint32_t color);

#endif // MLX_UTILS_H
