/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 21:53:30 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/15 20:52:29 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void			put_error(char *filename, char *add_info, int err_code)
{
	char *format;

	format = filename ? "fdf: %s: %s\n" : "fdf: %s\n";
	ft_printf(format, filename ? filename : add_info, add_info);
	exit(err_code);
}

static void		configure_mlx(t_fdf *core)
{
	if (!(core->window->mlx_ptr = mlx_init()))
		put_error(NULL, "Mlx init failed", MLX_ERROR);
	core->window->win_ptr = mlx_new_window(core->window->mlx_ptr,
			WIDTH, HEIGHT, WIN_NAME);
	if (!core->window->win_ptr)
		put_error(NULL, "Can't create window", MLX_ERROR);
	core->window->img.img = mlx_new_image(core->window->mlx_ptr, WIDTH, HEIGHT);
	if (!core->window->img.img)
		put_error(NULL, "Can't create image", MLX_ERROR);
	core->window->img.matrix = mlx_get_data_addr(core->window->img.img,
			&core->window->img.bpp, &core->window->img.size_line,
			&core->window->img.endian);
	core->window->img.bpp /= 8;
	mlx_hook(core->window->win_ptr, 2, 0, key_press, core);
	mlx_hook(core->window->win_ptr, 17, 0, closewin, core);
	mlx_hook(core->window->win_ptr, 4, 0, mouse_press, core);
	mlx_hook(core->window->win_ptr, 5, 0, mouse_release, core);
	mlx_hook(core->window->win_ptr, 6, 0, mouse_move, core);
}

void			draw(t_fdf *core)
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
				drawline(get_work_point(&core->matrix[y][x], core),
				get_work_point(&core->matrix[y][x + 1], core), core->window);
			if (core->matrix[y + 1])
				drawline(get_work_point(&core->matrix[y][x], core),
				get_work_point(&core->matrix[y + 1][x], core), core->window);
		}
	}
	mlx_put_image_to_window(core->window->mlx_ptr, core->window->win_ptr,
			core->window->img.img, 0, 0);
}

int				main(int ac, char **av)
{
	t_fdf		core;
	t_camera	camera;
	t_window	window;

	if (ac != 2)
	{
		ft_printf("Usage: ./fdf [map]\n");
		return (0);
	}
	ft_bzero(&core, sizeof(t_fdf));
	ft_bzero(&camera, sizeof(t_camera));
	ft_bzero(&window, sizeof(t_window));
	core.camera = &camera;
	core.window = &window;
	core.camera->zoom = 40;
	parse(av[1], &core);
	configure_mlx(&core);
	core.cur_projection = iso_projection;
	draw(&core);
	mlx_loop(core.window->mlx_ptr);
}
