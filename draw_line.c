#include "fdf.h"

void		put_img_pixel(t_image *img, int x, int y, unsigned color)
{
	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return ;
	*(int *)(img->matrix + (y * WIDTH + x) * img->bpp) = color;
}

void		print_line(t_point *one, t_point *two, t_window *win)
{
	int		y;
	int 	x;

	y = one->y - 1;
	while (++y < two->y + 1)
	{
		x = one->x - 1;
		while (++x < two->x + 1)
			put_img_pixel(&win->img, x, y, win->color);
	}
}

void		x_line(t_point *one, t_point *two, t_window *win, t_point *d)
{
	double	grad;
	double	intery;
	int 	x;

	grad = d->y / d->x;
	intery = one->y + grad;
	put_img_pixel(&win->img, one->x, one->y, win->color);
	x = one->x;
	while (++x < two->x)
	{
		put_img_pixel(&win->img, x, (int)intery, win->color);   // 1 - fPart(intery)
		put_img_pixel(&win->img, x, (int)intery + 1, win->color); // (int)(fpart(intery))
		intery += grad;
	}
	put_img_pixel(&win->img, two->x, two->y, win->color);
}

void		y_line(t_point *one, t_point *two, t_window *win, t_point *d)
{
	double	grad;
	double	interx;
	int 	y;

	grad = d->x / d->y;
	interx = one->x + grad;
	put_img_pixel(&win->img, one->x, one->y, win->color);
	y = one->y;
	while (++y < two->y)
	{
		put_img_pixel(&win->img, (int)interx, y, win->color);
		put_img_pixel(&win->img, (int)interx + 1, y, win->color ); // (int)(fpart(intery))
		interx += grad;
	}
	put_img_pixel(&win->img, two->x, two->y, win->color);
}

void		drawline(t_point one, t_point two, t_window *win)
{
	t_point d;

	d.x = (two.x > one.x) ? two.x - one.x : one.x - two.x;
	d.y = (two.y > one.y) ? two.y - one.y : one.y - two.y;
	if (d.x == 0.0 || d.y == 0.0)
		print_line(&one, &two, win);
	else if (d.y < d.x)
	{
		if (two.x < one.x)
			swap_point(&one, &two);
		x_line(&one, &two, win, &d);
	}
	else
	{
		if (two.y < one.y)
			swap_point(&one, &two);
		y_line(&one, &two, win, &d);
	}
}