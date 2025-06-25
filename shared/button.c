#include "app.h"
#include "button.h"
#include "mlx_utils.h"
#include <stdlib.h>
#include <string.h>

/* private struct */
struct s_button {
  t_ui_element  elem;
  char         *label;
  void        (*cb)(t_button*);
  /* you could cache an mlx_image_t* for background/text */
};

/* forward declarations */
static void btn_update(t_ui_element *e, mlx_t *mlx);
static void btn_render(t_ui_element *e, mlx_image_t *canvas);
static void btn_on_click(t_ui_element *e, int button, int mx, int my);

t_button *button_create(mlx_t *mlx,
                        int x, int y, int w, int h,
                        const char *label,
                        void (*cb)(t_button*))
{
  (void)mlx;
  t_button *btn = malloc(sizeof *btn);
  if (!btn) return NULL;

  memset(btn, 0, sizeof *btn);
  btn->label = strdup(label);
  btn->cb    = cb;

  btn->elem.x         = x;
  btn->elem.y         = y;
  btn->elem.w         = w;
  btn->elem.h         = h;
  btn->elem.visible   = true;
  btn->elem.update    = btn_update;
  btn->elem.render    = btn_render;
  btn->elem.on_click  = btn_on_click;
  btn->elem.userdata  = btn;

  return btn;
}

void button_destroy(void *userdata)
{
  t_button *btn = userdata;
  if (!btn) return;
  free(btn->label);
  free(btn);
}

t_ui_element *button_as_element(t_button *btn)
{
  return &btn->elem;
}

/* highlight on hover, detect clicks */
static void btn_update(t_ui_element *e, mlx_t *mlx)
{
  (void)mlx;
  (void)e;
  /* we could track hover state here, e.g. change bgcolor */
}

static void btn_render(t_ui_element *e, mlx_image_t *canvas)
{
  extern t_app g_app;

  t_button *btn = e->userdata;
  /* simple outline + label; replace with textured quad if you like */
  uint32_t bg = 0x444444FF;
  uint32_t fg = 0xFFFFFFFF;
  /* fill rect */
  for (int px = 0; px < e->w; ++px)
    for (int py = 0; py < e->h; ++py)
      mlx_put_pixel(canvas, e->x + px, e->y + py, bg);
  /* draw border */
  for (int px = 0; px < e->w; ++px) {
    mlx_put_pixel(canvas, e->x + px, e->y, fg);
    mlx_put_pixel(canvas, e->x + px, e->y + e->h - 1, fg);
  }
  for (int py = 0; py < e->h; ++py) {
    mlx_put_pixel(canvas, e->x, e->y + py, fg);
    mlx_put_pixel(canvas, e->x + e->w - 1, e->y + py, fg);
  }
  
  size_t  len        = strlen(btn->label);
  int     text_w     = (int)len * LETTER_PIXEL_SIZE_W;
  int     text_h     = LETTER_PIXEL_SIZE_H;
  int     text_x     = e->x + (e->w - text_w) / 2;
  int     text_y     = e->y + (e->h - text_h) / 2;

  mlx_put_string_to_image(g_app.mlx, canvas, btn->label, text_x, text_y);

  /* stub: draw text centered – replace with MLX42 text API */
  /* e.g. mlx_put_string(mlx, btn->label, center_x, center_y, fg); */
}

static void btn_on_click(t_ui_element *e, int button, int mx, int my)
{
  (void)mx; (void)my;
  if (button != MLX_MOUSE_BUTTON_LEFT) return;
  t_button *btn = e->userdata;
  if (btn->cb) btn->cb(btn);
}
