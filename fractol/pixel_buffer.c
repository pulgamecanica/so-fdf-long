#include "fractol/pixel_buffer.h"
#include <stdlib.h>
#include <string.h>

t_pixel_buffer *pixel_buffer_create(int width, int height) {
  t_pixel_buffer *buf = malloc(sizeof *buf);
  if (!buf) return NULL;
  buf->width = width;
  buf->height = height;
  buf->data = calloc(width * height, sizeof(t_pixel_info));
  if (!buf->data) {
    free(buf);
    return NULL;
  }
  return buf;
}

void pixel_buffer_clear(t_pixel_buffer *buf) {
  if (buf && buf->data)
    memset(buf->data, 0, sizeof(t_pixel_info) * buf->width * buf->height);
}

void pixel_buffer_destroy(t_pixel_buffer *buf) {
  if (!buf) return;
  free(buf->data);
  free(buf);
}
