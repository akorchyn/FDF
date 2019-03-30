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

t_point	get_work_point(t_point const *point, t_fdf *core)
{
	t_point res;

	static int i = 0;

	res = *point;
	res.x *= core->camera->zoom;
	res.y *= core->camera->zoom;
	res.z *= core->camera->zoom;
	core->cur_projection(&res);
	res.y += (double)(HEIGHT - core->rows * core->camera->zoom) / 2 + core->camera->x;
	res.x += (double)(WIDTH - core->columns * core->camera->zoom) / 2  + core->camera->y;
	i++;
	return res;
}

void loop(t_fdf *core)
{
	int x;
	int y;

	y = -1;
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
	ft_printf("%d\n", ft_len(core.matrix, 8));
	loop(&core);
	system("leaks fdf2");
	mlx_loop(core.window->mlx_ptr);
//	sleep(100);
}