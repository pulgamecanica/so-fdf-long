/* scroll_list.c */
#include "scroll_list.h"
#include "mlx_utils.h"
#include "app.h"
#include <stdlib.h>
#include <string.h>

struct s_scroll_list {
  t_ui_element  elem;
  mlx_t        *mlx;
  t_ui_element **items;
  size_t        count;
  size_t        capacity;
  int           item_w;
  int           item_h;
  int           visible_count;
  size_t        offset;
  int gap;
};

static void sl_update(t_ui_element *e, mlx_t* mlx) {
  extern t_app g_app;
  t_scroll_list *sl = e->userdata;

  if (g_app.scroll_y > 0 && sl->offset > 0)
    sl->offset--;
  else if (g_app.scroll_y < 0 && sl->offset + sl->visible_count < sl->count)
    sl->offset++;

  /* clamp offset */
  if (sl->offset + sl->visible_count > sl->count) {
    if (sl->count >= (size_t)sl->visible_count)
      sl->offset = sl->count - sl->visible_count;
    else
      sl->offset = 0;
  }
  /* update visible items */
  for (size_t i = sl->offset;
       i < sl->offset + (size_t)sl->visible_count && i < sl->count;
       ++i) {
    if (sl->items[i]->update)
      sl->items[i]->update(sl->items[i], mlx);
  }
}

static void sl_render(t_ui_element *e, mlx_image_t *canvas) {
  t_scroll_list *sl = e->userdata;
  int x = sl->elem.x;
  int y = sl->elem.y;
  int w = sl->elem.w;
  int h = sl->elem.h;
  int ih = sl->item_h;
  int gap = sl->gap;

  /* render visible items */
  for (size_t i = sl->offset;
       i < sl->offset + (size_t)sl->visible_count && i < sl->count;
       ++i) {
    t_ui_element *child = sl->items[i];
    child->x = x;
    child->y = y + (int)(i - sl->offset) * (ih + gap);
    child->render(child, canvas);
  }
  /* arrows and scrollbar if overflow */
  if (sl->count > (size_t)sl->visible_count) {
    const int arrow_margin = 5; // distance from border
    
    /* arrow */
    int ah = ih / 2;

    /* scrollbar */
    int sbh_zone = h - ah - gap; // height available for scrollbar
    int sbw = 5;
    int sbx = x + w - sbw;
    int sbh = h / 10;
    int sby = y + (int)( (double)sl->offset / (sl->count - sl->visible_count) * (sbh_zone - sbh) );

    /* arrow */
    int cx = x + w / 2 - (sbw / 2);

    /* draw up arrow */
    if (sl->offset >= 2) {
      for (int dy = 0; dy < ah; ++dy) {
        for (int dx = -dy; dx <= dy; ++dx) {
          int px = cx + dx;
          int py = y - ah + dy - arrow_margin; // offset upward
          my_mlx_put_pixel(canvas, px, py, 0xFFFFFFFF);
        }
      }
    }

    /* draw down arrow */
    if (sl->offset + sl->visible_count < sl->count) {
      for (int dy = 0; dy < ah; ++dy) {
        for (int dx = -dy; dx <= dy; ++dx) {
          int px = cx + dx;
          int py = y + h - dy + arrow_margin - gap;
          my_mlx_put_pixel(canvas, px, py, 0xFFFFFFFF);
        }
      }
    }

    /* clear vertical for scrollbar */
    int scroll_bar_padding_left = 6;
    for (int px = 0; px < sbw; ++px) {
      for (int py = 0; py < h; ++py) {
        my_mlx_put_pixel(canvas, sbx + px + scroll_bar_padding_left, y + py, 0x00000000); // fully transparent
      }
    }

    for (int px = 0; px < sbw; ++px)
      for (int py = 0; py < sbh; ++py)
        my_mlx_put_pixel(canvas, sbx + px + scroll_bar_padding_left, sby + py, 0x888888FF);
  }
}

static void sl_on_click(t_ui_element *e, int button, int mx, int my) {
  t_scroll_list *sl = e->userdata;
  if (button != MLX_MOUSE_BUTTON_LEFT)
    return;
  int rel_y = my - sl->elem.y;
  /* arrows */
  if ((size_t)sl->item_h/2 > 0 && sl->count > (size_t)sl->visible_count) {
    int ah = sl->item_h / 2;
    if (rel_y < ah) {
      if (sl->offset > 0)
        sl->offset--;
      return;
    }
    if (rel_y >= sl->elem.h - ah) {
      if (sl->offset + sl->visible_count < sl->count)
        sl->offset++;
      return;
    }
  }
  /* item click */
  int block_h = sl->item_h + sl->gap;
  int index = rel_y / block_h + sl->offset;
  
  int offset_in_block = rel_y % block_h;
  if (offset_in_block >= sl->item_h)
    return; // clicked in the gap, ignore

  if ((size_t)index < sl->count) {
    t_ui_element *child = sl->items[index];
    if (child->on_click)
      child->on_click(child, button, mx, my);
  }
}


static int sl_ensure_capacity(t_scroll_list *sl) {
  if (sl->count < sl->capacity)
    return 0;
  size_t newcap = sl->capacity == 0 ? 4 : sl->capacity * 2;
  t_ui_element **tmp = realloc(sl->items, newcap * sizeof *tmp);
  if (!tmp) return -1;
  sl->items = tmp;
  sl->capacity = newcap;
  return 0;
}


t_scroll_list *scroll_list_create(mlx_t *mlx,
                                  int x, int y,
                                  int w, int h,
                                  int item_w, int item_h,
                                  int gap)
{
  if (h < item_h) return NULL;
  t_scroll_list *sl = malloc(sizeof *sl);
  if (!sl) return NULL;
  memset(sl, 0, sizeof *sl);
  sl->mlx = mlx;
  sl->items = NULL;
  sl->count = sl->capacity = 0;
  sl->item_w = item_w;
  sl->item_h = item_h;
  sl->visible_count = (h + gap) / (item_h + gap);
  sl->offset = 0;
  sl->gap = gap;

  /* init base element */
  sl->elem.x = x;
  sl->elem.y = y;
  sl->elem.w = w;
  sl->elem.h = h;
  sl->elem.visible = true;
  sl->elem.update = sl_update;
  sl->elem.render = sl_render;
  sl->elem.on_click = sl_on_click;
  sl->elem.userdata = sl;

  return sl;
}

void scroll_list_destroy(void *userdata) {
  t_scroll_list *sl = userdata;
  free(sl->items);
  free(sl);
}

int scroll_list_add(t_scroll_list *sl, t_ui_element *elem) {
  if (elem->w != sl->item_w || elem->h != sl->item_h)
    return -1; // size mismatch
  if (sl_ensure_capacity(sl) < 0)
    return -1;
  sl->items[sl->count++] = elem;
  return 0;
}


t_ui_element *scroll_list_as_element(t_scroll_list *sl) {
  return &sl->elem;
}
