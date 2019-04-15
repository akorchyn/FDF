/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 20:53:17 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/15 20:57:58 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	top_projection(t_point *const original)
{
	(void)original;
}

void	x_rotation(t_point *point, t_fdf *core)
{
	double			y;
	double			z;
	const double	tmp = core->columns * core->camera->zoom / 2;
	const t_point	center = {tmp, core->rows * core->camera->zoom / 2, 0, 0};

	y = point->y - center.y;
	z = point->z - center.z;
	point->y = center.y + y * cos(TO_RADS(core->camera->x_angle))
		+ z * sin(TO_RADS(core->camera->x_angle));
	point->z = center.z + (-y) * sin(TO_RADS(core->camera->x_angle))
		+ z * cos(TO_RADS(core->camera->x_angle));
}

void	iso_projection(t_point *const original)
{
	double			previous_x;
	double			previous_y;

	previous_x = original->x;
	previous_y = original->y;
	original->x = (previous_x - previous_y) * cos(0.523599);
	original->y = -original->z + (previous_x + previous_y) * sin(0.523599);
}

void	y_rotation(t_point *point, t_fdf *core)
{
	double			z;
	double			x;
	const double	tmp = core->columns * core->camera->zoom / 2;
	const t_point	center = {tmp, core->rows * core->camera->zoom / 2, 0, 0};

	z = point->z - center.z;
	x = point->x - center.x;
	point->x = center.x + x * cos(TO_RADS(core->camera->y_angle))
		+ z * sin(TO_RADS(core->camera->y_angle));
	point->z = center.z + (-z) * sin(TO_RADS(core->camera->y_angle))
		+ z * cos(TO_RADS(core->camera->y_angle));
}

t_point	get_work_point(t_point const *point, t_fdf *core)
{
	t_point res;

	res = *point;
	res.x *= core->camera->zoom;
	res.y *= core->camera->zoom;
	res.z *= core->camera->zoom;
	res.color = point->color;
	;
	x_rotation(&res, core);
	y_rotation(&res, core);
	core->cur_projection(&res);
	;
	res.y += (double)(HEIGHT - core->rows * core->camera->zoom) / 2
			+ core->camera->y;
	res.x += (double)(WIDTH - core->columns * core->camera->zoom) / 2
			+ core->camera->x;
	return (res);
}
