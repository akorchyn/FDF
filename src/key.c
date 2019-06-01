/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 20:50:05 by akorchyn          #+#    #+#             */
/*   Updated: 2019/06/01 14:03:20 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "keys.h"

int		closewin(void *core)
{
	t_fdf	*x;
	int		i;

	x = core;
	mlx_destroy_image(x->window->mlx_ptr, x->window->img.img);
	mlx_destroy_window(x->window->mlx_ptr, x->window->win_ptr);
	i = -1;
	while (x->matrix[++i])
		free(x->matrix[i]);
	free(x->matrix);
	exit(0);
	return (0);
}

int		key_press(int key_code, void *param)
{
	t_fdf	*core;

	core = param;
	if (key_code == ESC)
		closewin(param);
	else if (key_code == KEY_1)
		core->cur_projection = top_projection;
	else if (key_code == KEY_2)
		core->cur_projection = iso_projection;
	else if (key_code == KEY_I)
		core->window->add_text_info = core->window->add_text_info ? 0 : 1;
	else if (key_code == SPACE)
		core->window->color_id += (core->window->color_id == 3) ? -3 : 1;
	if (key_code == KEY_1 || key_code == KEY_2)
	{
		core->camera->x_angle = 0;
		core->camera->y_angle = 0;
	}
	draw(core);
	return (1);
}

int		mouse_press(int key_code, int x, int y, void *param)
{
	t_fdf *core;

	core = param;
	if ((core->camera->zoom > 2 && key_code == MOUSE_SCROLL_DOWN)
			|| key_code == MOUSE_SCROLL_UP)
	{
		core->camera->zoom += (key_code == MOUSE_SCROLL_DOWN) ? -2 : 2;
		draw(core);
	}
	else if (key_code == MOUSE_LEFT_BUTTON)
	{
		core->left.x = x;
		core->left.y = y;
		core->left.button_clicked = 1;
	}
	else if (key_code == MOUSE_RIGHT_BUTTON)
	{
		core->right.x = x;
		core->right.y = y;
		core->right.button_clicked = 1;
	}
	return (0);
}

int		mouse_release(int key_code, int x, int y, void *param)
{
	t_fdf *core;

	(void)x;
	(void)y;
	core = param;
	if (key_code == MOUSE_LEFT_BUTTON)
		core->left.button_clicked = 0;
	else if (key_code == MOUSE_RIGHT_BUTTON)
		core->right.button_clicked = 0;
	return (0);
}

int		mouse_move(int x, int y, void *param)
{
	t_fdf	*core;

	core = param;
	if (core->left.button_clicked)
	{
		core->camera->x += core->left.x - x;
		core->camera->y += core->left.y - y;
		core->left.x = x;
		core->left.y = y;
	}
	else if (core->right.button_clicked)
	{
		core->camera->x_angle -= core->right.y - y;
		core->camera->y_angle -= core->right.x - x;
		core->right.x = x;
		core->right.y = y;
	}
	if (core->right.button_clicked || core->left.button_clicked)
		draw(core);
	return (0);
}
