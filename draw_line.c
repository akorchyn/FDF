#include "fdf.h"

static double fpart(double x)
{
	int y = x;

	return (x - y);
}

static double rfpart(double x)
{
	return (1 - fpart(x));
}

static void		start_line(t_point *one, t_point *two, t_window *win, t_woo *x)
{
	double xend;
	double xgap;
	double yend;

	x->gradient = (two->y - one->y) / (two->x - one->x);
	xend = round(one->x);
	yend = one->y + x->gradient * (xend - one->x);
	xgap = rfpart(one->x + 0.5);
	x->one.x = xend;
	x->one.y = (int)yend;
	mlx_pixel_put(win->mlx_ptr, win->win_ptr, x->one.x, x->one.y,
				  win->color * rfpart(yend) * xgap);
	mlx_pixel_put(win->mlx_ptr, win->win_ptr, one->x, x->one.y + 1,
				  win->color * fpart(yend) * xgap);
	x->intery = yend + x->gradient;
}

static void		end_line(t_point *two, t_window *win, t_woo *x)
{
	double xend;
	double xgap;
	double yend;

	xend = round(two->x);
	yend = two->y + x->gradient * (xend - two->x);
	xgap = fpart(two->x + 0.5);
	x->two.x = xend;
	x->two.y = (int)yend;
	mlx_pixel_put(win->mlx_ptr, win->win_ptr, x->two.x, x->two.y,
				  win->color * rfpart(yend) * xgap);
	mlx_pixel_put(win->mlx_ptr, win->win_ptr, x->two.x, x->two.y + 1,
				  win->color * fpart(yend) * xgap);
}

void		drawline(t_point one, t_point two, t_window *win)
{
	t_woo	x;
	int 	i;

	if (ABS(two.x - one.x) < ABS(two.y - one.y))
	{
		swap_float(&one.x, &one.y);
		swap_float(&two.x, &two.y);
	}
	if (two.x < one.x)
		swap_point(&one, &two);
	start_line(&one, &two, win, &x);
	end_line(&two, win, &x);
	i = (int)round(x.one.x);
	while (++i < x.two.x + 1)
	{
		mlx_pixel_put(win->mlx_ptr, win->win_ptr, i, (int)(x.intery),
					  win->color * rfpart(x.intery));
		mlx_pixel_put(win->mlx_ptr, win->win_ptr, i, (int)x.intery + 1,
					  win->color * fpart(x.intery));
		x.intery += x.gradient;
	}
}