#include "fractol/fractol_gameplay.h"
#include "fractol/numeric/backend.h"
#include "ui_manager.h"
#include "fractol/dashboard.h"
#include "fractol/fractol.h"
#include "mlx_utils.h"
#include "button.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DASHBOARD_BTN_SIZE 20

t_fractol *g_fractol = NULL;

static t_button *dashboard_btn = NULL;
static t_ui_manager ui;

static void on_toggle_dashboard(t_button *btn)
{
  (void)btn;
  if (g_fractol)
    g_fractol->show_dashboard = !g_fractol->show_dashboard;
}

static void init_button(t_app *app) {
  if (dashboard_btn) return;
  ui_manager_init(&ui);

  dashboard_btn = button_create(app->mlx, app->mlx->width - 10 - DASHBOARD_BTN_SIZE, 10, DASHBOARD_BTN_SIZE, DASHBOARD_BTN_SIZE, "", on_toggle_dashboard);
  button_set_background_path(dashboard_btn, HELP_BUTTON_BG_PATH);
  button_set_background_color(dashboard_btn, 0x424242FF);
  ui_manager_add(&ui, button_as_element(dashboard_btn));
}

void fractol_gameplay_on_enter(t_app *app) {
  printf("[Fractol] Entering gameplay scene\n");

  g_fractol = fractol_create(app->mlx->width, app->mlx->height); // or use app->width/app->height
  if (!g_fractol) {
    fprintf(stderr, "Failed to initialize fractol\n");
    return;
  }
  init_fractol_dashboard(app, g_fractol);
  mlx_image_to_window(app->mlx, g_fractol->canvas, 0, 0);
  init_button(app);
}

void fractol_gameplay_on_exit(t_app *app) {
  printf("[Fractol] Exiting gameplay scene\n");

  destroy_fractol_dashboard(app, g_fractol);
  fractol_destroy(g_fractol);
  button_destroy(dashboard_btn);
  ui_manager_destroy(&ui);
  g_fractol = NULL;
  dashboard_btn = NULL;
}

static void update_selection(t_app *app) {
  t_fractol *f = g_fractol;

  if (!f->paused && !f->show_dashboard) {
    if (mlx_is_mouse_down(app->mlx, MLX_MOUSE_BUTTON_LEFT)) {
      if (!f->is_selecting) {
        f->is_selecting = true;
        mlx_get_mouse_pos(app->mlx, &f->select_start_x, &f->select_start_y);
      }
      mlx_get_mouse_pos(app->mlx, &f->select_end_x, &f->select_end_y);

      // Apply aspect ratio preservation
      int sx = f->select_start_x;
      int sy = f->select_start_y;
      int ex = f->select_end_x;
      int ey = f->select_end_y;

      int dx = ex - sx;
      int dy = ey - sy;

      float canvas_aspect = (float)app->mlx->width / app->mlx->height;
      float rect_aspect = (float)abs(dx) / (float)(abs(dy) + 1e-5f); // avoid div by 0

      if (rect_aspect > canvas_aspect) {
        dy = (int)((float)abs(dx) / canvas_aspect);
        ey = (ey > sy) ? sy + dy : sy - dy;
      } else {
        dx = (int)((float)abs(dy) * canvas_aspect);
        ex = (ex > sx) ? sx + dx : sx - dx;
      }

      f->select_end_x = ex;
      f->select_end_y = ey;

      if(f->select_end_x <= 0)
        f->select_end_x = 0;
      if (f->select_end_y <= 0)
        f->select_end_y = 0;
      if (f->select_end_x >= app->mlx->width)
        f->select_end_x = app->mlx->width - 1;
      if (f->select_end_y >= app->mlx->height)
        f->select_end_y = app->mlx->height - 1;
    } else if (f->is_selecting) {
      f->is_selecting = false;

      const t_precision_backend *b = f->ctx->backend;
      t_coord *top_left = b->create(0, 0);
      t_coord *bottom_right = b->create(0, 0);

      int x1 = f->select_start_x;
      int y1 = f->select_start_y;
      int x2 = f->select_end_x;
      int y2 = f->select_end_y;

      if (MAX(x1, x2) - MIN(x1, x2) < 10 || MAX(y1, y2) - MIN(y1, y2) < 10)
        return ;

      b->screen_to_world(f->ctx, x1, y1, top_left);
      b->screen_to_world(f->ctx, x2, y2, bottom_right);

      double re1 = b->get_re(top_left);
      double im1 = b->get_im(top_left);
      double re2 = b->get_re(bottom_right);
      double im2 = b->get_im(bottom_right);

      // Compute new center
      f->ctx->view.center_x = (re1 + re2) * 0.5;
      f->ctx->view.center_y = (im1 + im2) * 0.5;

      // Compute new scale (preserve canvas aspect)
      double span_x = fabs(re2 - re1);
      double span_y = fabs(im2 - im1);
      float canvas_aspect = (float)app->mlx->width / app->mlx->height;

      double desired_span = span_x;
      if ((span_x / span_y) < canvas_aspect) {
          // too tall → use y
          desired_span = span_y * canvas_aspect;
      }

      f->ctx->view.scale = app->mlx->width / desired_span;
      f->ctx->dirty = true;
      f->ctx->force_redraw = true;

      b->destroy(top_left);
      b->destroy(bottom_right);

    }
  }
}

void fractol_gameplay_update(t_app *app) {
  if (!g_fractol)
    return;

  if (mlx_is_key_down(app->mlx, MLX_KEY_P))
    g_fractol->paused = !g_fractol->paused;
  if (mlx_is_key_down(app->mlx, MLX_KEY_H))
    g_fractol->show_dashboard = !g_fractol->show_dashboard;

  if (g_fractol->paused)
    return ;
  fractol_update(g_fractol);
  
  if (g_fractol->show_dashboard)
    update_fractol_dashboard(app, g_fractol);
  else {
    ui_manager_update(&ui, app->mlx);
    update_selection(app);
  }
}

void fractol_gameplay_render(t_app *app) {
  if (!g_fractol)
  return;
  
  fractol_render(g_fractol);

  if (g_fractol->show_dashboard) {
    render_fractol_dashboard(app, g_fractol);
  } else if (g_fractol->paused) {
    mlx_put_string_to_image(app->mlx, g_fractol->canvas, "Paused", app->mlx->width / 2,  app->mlx->height / 2);
  } else if (g_fractol->is_selecting) {
      int x1 = g_fractol->select_start_x;
      int y1 = g_fractol->select_start_y;
      int x2 = g_fractol->select_end_x;
      int y2 = g_fractol->select_end_y;
      draw_rectangle(g_fractol->canvas, x1, y1, x2, y2, 0xFFFFFFDD);
  }
  ui_manager_render(&ui, g_fractol->canvas);
}
