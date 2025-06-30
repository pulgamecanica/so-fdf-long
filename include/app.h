#ifndef APP_H
#define APP_H

#include <MLX42/MLX42.h>

/*
 * central application state;
 * only global is g_app, never used elsewhere
 */
typedef struct s_app {
  mlx_t  *mlx;
  int     selected_game;  // 0: fract-ol, 1: fdf, 2: so-long
  int     selected_map;   // per-game map/function index
  double  scroll_x;
  double  scroll_y;
  bool    size_ok;
  bool    should_resize;
} t_app;

/* forward */
void start(void);

#endif // APP_H
