#include "app.h"
#include "mlx_utils.h"
#include <glad/glad.h>    // MLX42 uses GLAD for its GL context
#include <GLFW/glfw3.h>   // ensure GLFW symbols are pulled in
#include <stdint.h>
#include <stdlib.h>

void mlx_set_background(mlx_t *mlx, uint32_t color)
{
  (void)mlx; // context is already bound by MLX42
  
  // Unpack bytes into normalized floats
  float r = ((color >> 24) & 0xFF) / 255.0f;
  float g = ((color >> 16) & 0xFF) / 255.0f;
  float b = ((color >>  8) & 0xFF) / 255.0f;
  float a = ((color >>  0) & 0xFF) / 255.0f;

  // Clear the color buffer
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

static int put_pixel_valid(mlx_image_t* img, uint32_t x, uint32_t y) {
  return (x < img->width && y < img->height && mlx_get_pixel(img, x, y) != 0);
}

static int get_rgba(int r, int g, int b, int a) {
  return (r << 24 | g << 16 | b << 8 | a);
}

int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y) {
  if (x > image->width || y > image->height)
    return 0xFF000000;
  uint8_t* pixelstart = image->pixels + (y * image->width + x) * BPP;
  return get_rgba(*(pixelstart), *(pixelstart + 1),
    * (pixelstart + 2), *(pixelstart + 3));
}

int32_t mlx_get_pixel_tex(mlx_texture_t* image, uint32_t x, uint32_t y) {
  if (x > image->width || y > image->height)
    return 0xFF000000;
  uint8_t* pixelstart = image->pixels + (y * image->width + x) * BPP;
  return get_rgba(*(pixelstart), *(pixelstart + 1),
    * (pixelstart + 2), *(pixelstart + 3));
}

void put_img_to_img(mlx_image_t *dest, mlx_image_t *src, int dst_x, int dst_y)
{
  for (uint32_t y = 0; y < src->height; ++y) {
    for (uint32_t x = 0; x < src->width; ++x) {
      if (put_pixel_valid(src, x, y))
        mlx_put_pixel(dest, dst_x + x, dst_y + y, mlx_get_pixel(src, x, y));
    }
  }
}

void mlx_put_string_to_image(mlx_t       *mlx,
                             mlx_image_t *dest,
                             const char  *str,
                             int          x,
                             int          y)
{
  // 1) render text into its own image
  mlx_image_t *txt = mlx_put_string(mlx, str, 0, 0);
  if (!txt)
    return;

  // 2) blit into our canvas
  put_img_to_img(dest, txt, x, y);

  // 3) clean up
  mlx_delete_image(mlx, txt);
}

void draw_rectangle(mlx_image_t *img, int x1, int y1, int x2, int y2, uint32_t color) {
  extern t_app g_app;
  
  int min_x = MIN(x1, x2), max_x = MAX(x1, x2);
  int min_y = MIN(y1, y2), max_y = MAX(y1, y2);

  for (int x = min_x; x <= max_x; ++x) {
    if (x < 0 || x >= g_app.mlx->width || x >= (int)img->width)
      continue;
    mlx_put_pixel(img, x, min_y, color);
    mlx_put_pixel(img, x, max_y, color);
  }
  for (int y = min_y; y <= max_y; ++y) {
    if (y < 0 || y >= g_app.mlx->width || y  >= (int)img->height)
      continue;
    mlx_put_pixel(img, min_x, y, color);
    mlx_put_pixel(img, max_x, y, color);
  }
}

mlx_image_t *util_resize_texture_to_image(t_app *app,
                                          mlx_texture_t **texture,
                                          const char *path,
                                          mlx_image_t **image_out) {
  if (!texture || !app)
    return NULL;

  // Load texture if needed
  if (*texture == NULL) {
    *texture = mlx_load_png(path);
    if (!*texture)
      return NULL;
  }

  // Destroy existing image if needed
  if (image_out && *image_out) {
    mlx_delete_image(app->mlx, *image_out);
    *image_out = NULL;
  }

  mlx_image_t *img = mlx_new_image(app->mlx, app->mlx->width, app->mlx->height);
  if (!img) {
    mlx_delete_texture(*texture);
    return NULL;
  }

  for (int y = 0; y < (int)app->mlx->height; ++y) {
    for (int x = 0; x < (int)app->mlx->width; ++x) {
      int src_x = x * (*texture)->width / app->mlx->width;
      int src_y = y * (*texture)->height / app->mlx->height;
      uint32_t color = mlx_get_pixel_tex(*texture, src_x, src_y);

      // Decompose
      uint8_t r = (color >> 24) & 0xFF;
      uint8_t g = (color >> 16) & 0xFF;
      uint8_t b = (color >> 8)  & 0xFF;
      // Adjust alpha
      uint8_t a = 180;

      // Repack
      color = (r << 24) | (g << 16) | (b << 8) | a;

      mlx_put_pixel(img, x, y, color);
    }
  }

  if (image_out)
    *image_out = img;

  return img;
}
