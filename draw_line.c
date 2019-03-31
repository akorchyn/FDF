#include "fdf.h"

void		put_img_pixel(t_image *img, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return ;
	*(int *)(img->matrix + (y * WIDTH + x) * img->bpp) = color;
}

//void		print_line(t_point *one, t_point *two, t_window *win)
//{
//	int		y;
//	int 	x;
//
//	y = one->y - 1;
//	while (++y < two->y + 1)
//	{
//		x = one->x - 1;
//		while (++x < two->x + 1)
//			put_img_pixel(&win->img, x, y, win->color);
//	}
//}
//
//void		x_line(t_point *one, t_point *two, t_window *win, t_point *d)
//{
//	double	grad;
//	double	intery;
//	int 	x;
//
//	grad = d->y / d->x;
//	intery = one->y + grad;
//	put_img_pixel(&win->img, one->x, one->y, win->color);
//	x = one->x;
//	while (++x < two->x)
//	{
//		put_img_pixel(&win->img, x, (int)intery, win->color);   // 1 - fPart(intery)
//		put_img_pixel(&win->img, x, (int)intery + 1, win->color); // (int)(fpart(intery))
//		intery += grad;
//	}
//	put_img_pixel(&win->img, two->x, two->y, win->color);
//}
//
//void		y_line(t_point *one, t_point *two, t_window *win, t_point *d)
//{
//	double	grad;
//	double	interx;
//	int 	y;
//
//	grad = d->x / d->y;
//	interx = one->x + grad;
//	put_img_pixel(&win->img, one->x, one->y, win->color);
//	y = one->y;
//	while (++y < two->y)
//	{
//		put_img_pixel(&win->img, (int)interx, y, win->color);
//		put_img_pixel(&win->img, (int)interx + 1, y, win->color ); // (int)(fpart(intery))
//		interx += grad;
//	}
//	put_img_pixel(&win->img, two->x, two->y, win->color);
//}
//
//void		drawline(t_point one, t_point two, t_window *win)
//{
//	t_point d;
//
//	d.x = (two.x > one.x) ? two.x - one.x : one.x - two.x;
//	d.y = (two.y > one.y) ? two.y - one.y : one.y - two.y;
//	if (d.x == 0.0 || d.y == 0.0)
//		print_line(&one, &two, win);
//	else if (d.y < d.x)
//	{
//		if (two.x < one.x)
//			swap_point(&one, &two);
//		x_line(&one, &two, win, &d);
//	}
//	else
//	{
//		if (two.y < one.y)
//			swap_point(&one, &two);
//		y_line(&one, &two, win, &d);
//	}
//}

typedef struct s_brath
{
	int x1;
	int y1;
	int x2;
	int y2;
	int deltax;
	int deltay;
	int shiftx;
	int shifty;
	int error;
	int error2;
}				t_brath;

t_brath			fill(t_point *one, t_point *two)
{
	t_brath	x;

	x.x1 = (int)round(one->x);
	x.x2 = (int)round(two->x);
	x.y1 = (int)round(one->y);
	x.y2 = (int)round(two->y);
	x.deltax = ABS(x.x2 - x.x1);
	x.deltay = ABS(x.y2 - x.y1);
	x.shiftx = x.x1 < x.x2 ? 1 : -1;
	x.shifty = x.y1 < x.y2 ? 1 : -1;
	return (x);
}

void drawline(t_point one, t_point two, t_window *win)
{
	t_brath				x;

	x = fill(&one, &two);
	x.error = x.deltax - x.deltay;
	while (x.x1 != x.x2 || x.y1 != x.y2)
	{
		put_img_pixel(&win->img, x.x1, x.y1, win->color);
		x.error2 = x.error * 2;

		if (x.error2 > -x.deltay)
		{
			x.error -= x.deltay;
			x.x1 += x.shiftx;
		}
		if (x.error2 < x.deltax)
		{
			x.error += x.deltax;
			x.y1 += x.shifty;
		}
	}
}