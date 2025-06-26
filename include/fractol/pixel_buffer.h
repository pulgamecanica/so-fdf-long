#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct s_pixel_info {
  uint32_t iterations;
  bool     done;
} t_pixel_info;

typedef struct s_pixel_buffer {
  t_pixel_info *data;
  int           width;
  int           height;
} t_pixel_buffer;

t_pixel_buffer *pixel_buffer_create(int width, int height);
void pixel_buffer_clear(t_pixel_buffer *buf);
void pixel_buffer_destroy(t_pixel_buffer *buf);

#endif
