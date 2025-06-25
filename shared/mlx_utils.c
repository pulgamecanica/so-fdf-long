#include "mlx_utils.h"
#include <glad/glad.h>    // MLX42 uses GLAD for its GL context
#include <GLFW/glfw3.h>   // ensure GLFW symbols are pulled in
#include <stdint.h>

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
