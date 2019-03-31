/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 21:53:30 by akorchyn          #+#    #+#             */
/*   Updated: 2019/03/30 23:54:36 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void		put_error(char *filename, char *add_info, int err_code)
{
	char *format;

	format = filename ? "fdf: %s: %s\n" : "fdf: %s\n";
	ft_printf(format, filename ? filename : add_info, add_info);
	exit(err_code);
}

void configure_mlx(t_window *window)
{
	if (!(window->mlx_ptr = mlx_init()))
		put_error(NULL, "Mlx init failed", MLX_ERROR);
	window->win_ptr = mlx_new_window(window->mlx_ptr, WIDTH, HEIGHT, WIN_NAME);
	if (!window->win_ptr)
		put_error(NULL, "Can't create window", MLX_ERROR);
	window->img.img = mlx_new_image(window->mlx_ptr, WIDTH, HEIGHT);
	if (!window->img.img)
		put_error(NULL, "Can't create image", MLX_ERROR);
	window->img.matrix = mlx_get_data_addr(window->img.img, &window->img.bpp,
			&window->img.size_line, &window->img.endian);
	window->img.bpp /= 8;
}

void	iso(t_point * const original)
{
	double			previous_x;
	double			previous_y;

	previous_x = original->x;
	previous_y = original->y;
	original->x = (previous_x - previous_y) * cos(TO_RADS(30));
	original->y = ((previous_x + previous_y) * sin(TO_RADS(30)) - original->z);
}

void	x_rotation(t_point *point, t_fdf *core)
{
	double			y;
	double			z;
	const t_point	center = {core->columns * core->camera->zoom / 2,
								core->rows * core->camera->zoom / 2};

	y = point->y - center.y;
	z = point->z - center.z;
	core->camera->x_angle = (core->camera->x_angle >= 0) 
						? core->camera->x_angle % 360
						: 360 - (ABS(core->camera->x_angle) % -360);
	point->y = center.y + y * cos(TO_RADS(core->camera->x_angle))
				+ z * sin(TO_RADS(core->camera->x_angle));
	point->z = center.z + (-y) * sin(TO_RADS(core->camera->x_angle))
				+ z * cos(TO_RADS(core->camera->x_angle));
}

void	y_rotation(t_point *point, t_fdf *core)
{
	double			z;
	double			x;
	const t_point	center = {core->columns * core->camera->zoom / 2,
								core->rows * core->camera->zoom / 2};

	z = point->z - center.z;
	x = point->x - center.x;
	core->camera->y_angle = (core->camera->y_angle >= 0) 
						? core->camera->y_angle % 360
						: 360 - (ABS(core->camera->y_angle) % -360);
	point->x = center.x + x * cos(TO_RADS(core->camera->y_angle))
				+ z * sin(TO_RADS(core->camera->x_angle));
	point->z = center.z + (-z) * sin(TO_RADS(core->camera->x_angle))
				+ z * cos(TO_RADS(core->camera->x_angle));
}

void	z_rotation(t_point *point, t_fdf *core)
{
	double			y;
	double			x;
	const t_point	center = {core->columns * core->camera->zoom / 2,
								core->rows * core->camera->zoom / 2};

	y = point->z - center.z;
	x = point->x - center.x;
	core->camera->z_angle = (core->camera->z_angle >= 0) 
						? core->camera->z_angle % 360
						: 360 - (ABS(core->camera->z_angle) % -360);
	point->x = center.x + x * cos(TO_RADS(core->camera->z_angle))
				- y * sin(TO_RADS(core->camera->x_angle));
	point->y = center.y + x * sin(TO_RADS(core->camera->z_angle))
				+ y * cos(TO_RADS(core->camera->x_angle));
}

t_point	get_work_point(t_point const *point, t_fdf *core)
{
	t_point res;

	res = *point;
	res.x *= core->camera->zoom;
	res.y *= core->camera->zoom;
	res.z *= core->camera->zoom;
	// if (core->camera->x_angle)
	// 	x_rotation(&res, core);
	// if (core->camera->y_angle)
	// 	y_rotation(&res, core);
	// if (core->camera->z_angle)
	// 	z_rotation(&res, core);
	core->cur_projection(&res);

	res.y += (double)(HEIGHT - core->rows * core->camera->zoom) / 2 + core->camera->y;
	res.x += (double)(WIDTH - core->columns * core->camera->zoom) / 2  + core->camera->x;
	return res;
}

void draw(t_fdf *core)
{
	int x;
	int y;

	y = -1;
	ft_bzero(core->window->img.matrix, WIDTH * HEIGHT * core->window->img.bpp);
	while (core->matrix[++y])
	{
		x = -1;
		while (++x < core->columns)
		{
			if (x != core->columns - 1)
				drawline(get_work_point(&core->matrix[y][x + 1], core),
					get_work_point(&core->matrix[y][x], core), core->window);
			if (core->matrix[y + 1])
				drawline(get_work_point(&core->matrix[y + 1][x], core),
					get_work_point(&core->matrix[y][x], core), core->window);
		}
	}
	mlx_put_image_to_window(core->window->mlx_ptr, core->window->win_ptr,
			core->window->img.img, 0, 0);
}

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
	system("leaks a.out");
	exit(0);
	return (0);
}

int		key_press(int key_code, void *param)
{
	t_fdf *core;

	core = param;
	if (key_code == 123 || key_code == 124)
		core->camera->x_angle += (key_code == 123) ? -2 : 2;
	else if (key_code == 125 || key_code == 126)
		core->camera->z_angle += (key_code == 125) ? -2 : 2;
	else if (key_code == 53)
		closewin(param);
	draw(core);
	return (1);
}

int		mouse_press(int key_code, int x, int y, void *param)
{
	t_fdf *core;

	core = param;
	if ((core->camera->zoom >= 2 && key_code == 4) || key_code == 5)
	{
		core->camera->zoom += (key_code == 4) ? -2 : 2;
		draw(core);
	}
	else if (key_code == 1)
	{
		core->left.x = x;
		core->left.y = y;
		core->left.button_clicked = 1;
	}
	else if (key_code == 2)
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

	core = param;
	if (key_code == 1)
		core->left.button_clicked = 0;
	else if (key_code == 2)
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
		core->camera->y_angle += core->right.y - y;
		core->camera->x_angle += core->right.x - x;
		core->right.x = x;
		core->right.y = y;
	}
	if (core->right.button_clicked || core->left.button_clicked)
		draw(core);
	return (0);
}

void	handle_keys(t_fdf *core)
{
	mlx_hook(core->window->win_ptr, 2, 0, key_press, core);
	mlx_hook(core->window->win_ptr, 17, 0, closewin, core);
	mlx_hook(core->window->win_ptr, 4, 0, mouse_press, core);
	mlx_hook(core->window->win_ptr, 5, 0, mouse_release, core);
	mlx_hook(core->window->win_ptr, 6, 0, mouse_move, core);
}

int main(int ac, char **av)
{
	t_fdf		core;
	t_camera	camera;
	t_window	window;

	ft_bzero(&core, sizeof(t_fdf));
	if (ac != 2)
	{
		ft_printf("Usage: ./fdf [map]\n");
		return (0);
	}
	core.camera = &camera;
	core.window = &window;
	core.camera->zoom = 25;
	core.camera->x = 0;
	core.camera->y = 0;
	parse(av[1], &core);
	configure_mlx(core.window);
	core.window->color = 0xff0000;
	core.cur_projection = iso;
	draw(&core);
	handle_keys(&core);
	system("leaks fdf2");
	mlx_loop(core.window->mlx_ptr);
//	sleep(100);
}