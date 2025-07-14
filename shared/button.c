#include "mlx_utils.h"
#include "app.h"
#include "button.h"
#include "mlx_utils.h"
#include <MLX42/MLX42.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* private struct */
struct s_button {
  t_ui_element  elem;
  char         *label;
  void        (*cb)(t_button*);
  mlx_image_t  *bg_image;
  int           bg_color;
  bool          bg_set;
  bool          hovered;
  /* you could cache an mlx_image_t* for background/text */
};

/* forward declarations */
static void btn_update(t_ui_element *e, mlx_t *mlx);
static void btn_render(t_ui_element *e, mlx_image_t *canvas);
static void btn_on_click(t_ui_element *e, int button, int mx, int my);

extern t_app g_app;

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
  btn->bg_image  = NULL;
  btn->hovered = false;

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

bool button_set_background_path(t_button *btn, const char *path)
{
    if (!btn || !path) return false;

    mlx_t *mlx = g_app.mlx;

    mlx_texture_t *original = mlx_load_png(path);
    if (!original) return false;

    mlx_image_t *resized = mlx_new_image(mlx, btn->elem.w, btn->elem.h);
    if (!resized) {
      mlx_delete_texture(original);  // cleanup original
      return false;
    }

    for (int y = 0; y < btn->elem.h; ++y) {
        for (int x = 0; x < btn->elem.w; ++x) {
            int src_x = x * original->width / btn->elem.w;
            int src_y = y * original->height / btn->elem.h;
            uint32_t color = mlx_get_pixel_tex(original, src_x, src_y);
            my_mlx_put_pixel(resized, x, y, color);
        }
    }

    if (btn->bg_image)
        mlx_delete_image(mlx, btn->bg_image);

    btn->bg_image = resized;

    mlx_delete_texture(original);  // cleanup original

    return true;
}


void button_destroy(void *userdata)
{
  t_button *btn = userdata;
  if (!btn) return;
 
  if (btn->bg_image)
   mlx_delete_image(g_app.mlx, btn->bg_image);

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
  int mx, my;
  mlx_get_mouse_pos(mlx, &mx, &my);

  t_button *btn = e->userdata;

  bool inside =
    mx >= e->x &&
    my >= e->y &&
    mx <  e->x + e->w &&
    my <  e->y + e->h;

  btn->hovered = inside; 
  /* we could track hover state here, e.g. change bgcolor */
}

static void btn_render(t_ui_element *e, mlx_image_t *canvas)
{

  t_button *btn = e->userdata;
  /* simple outline + label; replace with textured quad if you like */
  uint32_t bg = 0x444444FF;
  uint32_t fg = btn->hovered ? 0xA258C0FF : 0xFFFFFFFF;

  if (btn->bg_set)
    for (int px = 0; px < e->w; ++px)
      for (int py = 0; py < e->h; ++py)
        my_mlx_put_pixel(canvas, e->x + px, e->y + py, btn->bg_color);

  if (btn->bg_image) {
    put_img_to_img(canvas, btn->bg_image, e->x, e->y);
  } else {
    /* fill rect */
    for (int px = 0; px < e->w; ++px)
      for (int py = 0; py < e->h; ++py)
        my_mlx_put_pixel(canvas, e->x + px, e->y + py, bg);
  }
  /* draw border */
  for (int px = 0; px < e->w; ++px) {
    my_mlx_put_pixel(canvas, e->x + px, e->y, fg);
    my_mlx_put_pixel(canvas, e->x + px, e->y + e->h - 1, fg);
  }
  for (int py = 0; py < e->h; ++py) {
    my_mlx_put_pixel(canvas, e->x, e->y + py, fg);
    my_mlx_put_pixel(canvas, e->x + e->w - 1, e->y + py, fg);
  }
  
  size_t  len        = strlen(btn->label);
  int     text_w     = (int)len * LETTER_PIXEL_SIZE_W;
  int     text_h     = LETTER_PIXEL_SIZE_H;
  int     text_x     = e->x + (e->w - text_w) / 2;
  int     text_y     = e->y + (e->h - text_h) / 2;

  mlx_put_string_to_image(g_app.mlx, canvas, btn->label, text_x, text_y);

  if (btn->hovered) {
    for (int px = 0; px < e->w; ++px) {
      for (int py = 0; py < e->h; ++py) {
        int cx = e->x + px;
        int cy = e->y + py;
        uint32_t dst = mlx_get_pixel(canvas, cx, cy);

        // blend (simple add)
        uint16_t r = ((dst >> 24) & 0xFF) + 20;
        uint16_t g = ((dst >> 16) & 0xFF) + 20;
        uint16_t b = ((dst >> 8)  & 0xFF) + 20;
        uint16_t a = 0xFF;

        r = r > 255 ? 255 : r;
        g = g > 255 ? 255 : g;
        b = b > 255 ? 255 : b;

        uint32_t blended = (r << 24) | (g << 16) | (b << 8) | a;
        my_mlx_put_pixel(canvas, cx, cy, blended);
      }
    }
  }
}

void button_set_background_color(t_button *btn, int color) {
  btn->bg_set = true;
  btn->bg_color = color;
}

static void btn_on_click(t_ui_element *e, int button, int mx, int my)
{
  (void)mx; (void)my;
  if (button != MLX_MOUSE_BUTTON_LEFT) return;
  t_button *btn = e->userdata;
  if (btn->cb) btn->cb(btn);
}
