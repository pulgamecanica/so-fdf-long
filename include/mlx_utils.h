// background.h
#ifndef MLX_UTILS_H
#define MLX_UTILS_H

#include "app.h"
#include <MLX42/MLX42.h>
#include <stdint.h>

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#define BPP sizeof(int32_t)
#define HEX_TO_RGBA(rgb_hex) (((rgb_hex & 0xFF0000) << 8) | ((rgb_hex & 0x00FF00) << 8) | ((rgb_hex & 0x0000FF) << 8) | 0xFF)

/**
 * Clear the entire window to the given RGBA color.
 *
 * @param mlx   The MLX42 instance (holds the GL context).
 * @param color 0xRRGGBBAA packed color.
 */
void mlx_set_background(mlx_t *mlx, uint32_t color);


/**
 * Blit `src` onto `dest` at (dst_x, dst_y), skipping fully‐transparent pixels.
 *
 * @param dest    The canvas image you clear each frame.
 * @param src     A small image returned by mlx_put_string().
 * @param dst_x   X offset in dest.
 * @param dst_y   Y offset in dest.
 */
void put_img_to_img(mlx_image_t *dest, mlx_image_t *src, int dst_x, int dst_y);

/**
 * In MLX42 Colors are as follows:
 * 0000 0000 A (1st byte) -> uint8_t because it's 8 bits
 * 0000 0000 R (2nd byte)
 * 0000 0000 G (3rd byte)
 * 0000 0000 B (4th byte)
 **/
int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y);
int32_t mlx_get_pixel_tex(mlx_texture_t* image, uint32_t x, uint32_t y);

/**
 * Draw `str` into `dest` at (x,y):
 *   1) creates a temporary mlx_image_t* via mlx_put_string
 *   2) blits it onto `dest`
 *   3) deletes the temporary image
 */
void mlx_put_string_to_image(mlx_t       *mlx,
                             mlx_image_t *dest,
                             const char  *str,
                             int          x,
                             int          y);


void draw_rectangle(mlx_image_t *img, int x1, int y1, int x2, int y2, uint32_t color);


/**
 * Resize a texture into a new MLX image matching the app window size.
 *
 * Behavior:
 *   1) If `*texture == NULL`, loads it from `path` via mlx_load_png.
 *   2) If `*image_out != NULL`, destroys the existing image.
 *   3) Creates a new image using app->mlx dimensions.
 *   4) Performs nearest-neighbor resizing from texture to image.
 *   5) Alpha channel is overridden to 180 for all pixels.
 *
 *  @param app:        Pointer to the application context (must not be NULL).
 *  @param texture:    Pointer to a texture pointer; loaded if NULL.
 *  @param path:       Path to PNG to load if `*texture` is NULL.
 *  @param image_out:  Optional image output pointer; will be destroyed and replaced.
 *
 *  @returns  Newly created image (also stored in `*image_out` if non-NULL),
 *     or NULL on error (e.g., failed texture or image allocation).
 */
mlx_image_t *util_resize_texture_to_image(t_app *app,
                                          mlx_texture_t **texture,
                                          const char *path,
                                          mlx_image_t **image_out);

void  my_mlx_put_pixel(mlx_image_t* image, int32_t x, int32_t y, uint32_t color);                                  
void  my_mlx_put_pixel_alpha(mlx_image_t* image, int32_t x, int32_t y, uint32_t color);                                  

#endif // MLX_UTILS_H
