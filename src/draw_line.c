/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 20:48:47 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/15 20:49:48 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

static void		put_img_pixel(t_image *img, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return ;
	*(int *)(img->matrix + (y * WIDTH + x) * img->bpp) = color;
}

static t_brath	fill(t_point *one, t_point *two)
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

void			drawline(t_point one, t_point two, t_window *win)
{
	t_brath				x;

	x = fill(&one, &two);
	x.error = x.deltax - x.deltay;
	while (x.x1 != x.x2 || x.y1 != x.y2)
	{
		put_img_pixel(&win->img, x.x1, x.y1, one.color);
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
