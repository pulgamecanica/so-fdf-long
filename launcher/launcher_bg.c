#include "launcher_bg.h"
#include <MLX42/MLX42.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define MAX_PARTICLES 500000
#define REPULSION_STRENGTH 1.0f
#define REPULSION_RAD 1000.0f

typedef struct s_particle {
  float x, y;
  float vx, vy;
} t_particle;

static mlx_texture_t* bg_texture = NULL;
static mlx_image_t*   bg_image = NULL;
static mlx_image_t* canvas = NULL;
static t_particle particles[MAX_PARTICLES];

static float randf(float min, float max) {
  return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void init_bg_image(t_app* app) {
  char path[64];

  snprintf(path, sizeof(path), "assets/bg%d.png", 1 + rand() % 5);
  bg_texture = mlx_load_png(path);
  if (!bg_texture) {
    fprintf(stderr, "Failed to load texture: %s\n", path);
    puts(mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
  }

  int win_w = app->mlx->width;
  int win_h = app->mlx->height;

  float tex_aspect = (float)bg_texture->width / bg_texture->height;
  float win_aspect = (float)win_w / win_h;

  // Determine scaled dimensions
  int draw_w, draw_h;
  if (win_aspect > tex_aspect) {
    draw_w = win_w;
    draw_h = (int)(win_w / tex_aspect);
  } else {
    draw_h = win_h;
    draw_w = (int)(win_h * tex_aspect);
  }

  bg_image = mlx_new_image(app->mlx, win_w, win_h);
  if (!bg_image || mlx_image_to_window(app->mlx, bg_image, 0, 0) < 0) {
    puts(mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
  }
  mlx_set_instance_depth(&bg_image->instances[0], 1);

  // Draw scaled texture into bg_image (nearest-neighbor scaling)
  int offset_x = (draw_w - win_w) / 2;
  int offset_y = (draw_h - win_h) / 2;

  for (int y = 0; y < win_h; ++y) {
    for (int x = 0; x < win_w; ++x) {
      float tex_x = (float)(x + offset_x) * bg_texture->width / draw_w;
      float tex_y = (float)(y + offset_y) * bg_texture->height / draw_h;

      int tx = (int)tex_x;
      int ty = (int)tex_y;


      if (tx >= 0 && tx < (int)bg_texture->width && ty >= 0 && ty < (int)bg_texture->height) {
        int tex_index = (ty * bg_texture->width + tx) * 4;
        uint8_t r = bg_texture->pixels[tex_index + 0] * 0.5f;
        uint8_t g = bg_texture->pixels[tex_index + 1] * 0.5f;
        uint8_t b = bg_texture->pixels[tex_index + 2] * 0.5f;
        uint8_t a = bg_texture->pixels[tex_index + 3];
        uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;
        mlx_put_pixel(bg_image, x, y, color);
      }
    }
  }
}

void launcher_bg_init(t_app* app) {
  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;

  canvas = mlx_new_image(app->mlx, WIDTH, HEIGHT);
  if (!canvas || mlx_image_to_window(app->mlx, canvas, 0, 0) < 0) {
    mlx_close_window(app->mlx);
    exit(EXIT_FAILURE);
  }
  mlx_set_instance_depth(&canvas->instances[0], 2);

  for (int i = 0; i < MAX_PARTICLES; ++i) {
    particles[i].x = randf(0, WIDTH);
    particles[i].y = randf(0, HEIGHT);
    particles[i].vx = randf(-0.5f, 0.5f);
    particles[i].vy = randf(-0.5f, 0.5f);
  }
  init_bg_image(app);
}

void launcher_bg_update(t_app* app) {
  const int WIDTH = app->mlx->width;
  const int HEIGHT = app->mlx->height;

  bool mouse_pressed = mlx_is_mouse_down(app->mlx, MLX_MOUSE_BUTTON_LEFT);
  const float repultion_rad = mouse_pressed ? REPULSION_RAD * 1.75 : REPULSION_RAD;
  memset(canvas->pixels, 0, WIDTH * HEIGHT * sizeof(int32_t));

  int mx, my;
  mlx_get_mouse_pos(app->mlx, &mx, &my);

  for (int i = 0; i < MAX_PARTICLES; ++i) {
    t_particle* p = &particles[i];

    float dx = p->x - (float)mx;
    float dy = p->y - (float)my;
    float dist_sq = dx * dx + dy * dy + 0.01f;

    if (dist_sq < repultion_rad) {
      float strength = REPULSION_STRENGTH / dist_sq;
      p->vx += dx * strength;
      p->vy += dy * strength;
    }

    p->x += p->vx;
    p->y += p->vy;

    p->vx *= 0.95f;
    p->vy *= 0.95f;

    if (p->x < 0) p->x = WIDTH - 1;
    if (p->x >= WIDTH) p->x = 0;
    if (p->y < 0) p->y = HEIGHT - 1;
    if (p->y >= HEIGHT) p->y = 0;

    int px = (int)p->x;
    int py = (int)p->y;
    if (px >= 0 && py >= 0 && px < WIDTH && py < HEIGHT)
      mlx_put_pixel(canvas, px, py, 0xFFFFFFFF);
  }
}

void launcher_bg_destroy(t_app* app) {
  if (bg_image)
    mlx_delete_image(app->mlx, bg_image);
  if (bg_texture)
    mlx_delete_texture(bg_texture);

  // You could free the image here if needed:
  // if (canvas) mlx_delete_image(app->mlx, canvas);
}
