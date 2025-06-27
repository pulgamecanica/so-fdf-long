#ifndef FRACTOL_DASHBOARD_H
#define FRACTOL_DASHBOARD_H

#include "app.h"
#include "fractol/fractol.h"

void init_fractol_dashboard(t_app *app, const t_fractol *f);
void update_fractol_dashboard(t_app *app, const t_fractol *ctx);
void render_fractol_dashboard(t_app *app, const t_fractol *ctx);
void destroy_fractol_dashboard(t_app *app, const t_fractol *f);

#endif
