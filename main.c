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
}

void	iso(t_point * const original)
{
	double			previous_x;
	double			previous_y;

	previous_x = original->x;
	previous_y = original->y;
	original->x = (previous_x + previous_y) * cos(TO_RADS(30));
	original->y = ((previous_x - previous_y) * sin(TO_RADS(30)) + original->z);
}

t_point	get_work_point(t_point const *point, t_fdf *core)
{
	t_point res;
	static int i = 0;

	res = *point;
	res.x *= core->camera->zoom;
	res.y *= core->camera->zoom;
	res.z *= core->camera->zoom;
	res.x += 400.0;
	res.y += 50.0;
	ft_printf("%d -> %F %F\n", i, res.x, res.y);
	core->cur_projection(&res);
	i++;
	return res;
}

void translate_cordinates(t_fdf *core)
{
	int size;
	int i;
	int j;

	i = -1;
	size = core->rows + 1;
	if (!(core->work = (t_point **)malloc(sizeof(t_point *) * size)))
		put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
	while (++i < size - 1)
	{
		j = -1;
		if (!(core->work[i] = malloc(sizeof(t_point) * core->columns)))
			put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
		while (++j < core->columns)
			core->work[i][j] = get_work_point(&core->matrix[i][j], core);
	}
	core->work[i] = NULL;
}

void loop(t_fdf *core)
{
	int x;
	int y;

	y = -1;
	translate_cordinates(core);
	while (core->work[++y])
	{
		x = -1;
		while (++x < core->columns)
		{
			if (x != core->columns - 1)
				drawline(core->work[y][x], core->work[y][x + 1], &core->window);
			if (core->work[y + 1])
				drawline(core->work[y][x], core->work[y + 1][x], &core->window);
		}
	}
}

int main(int ac, char **av)
{
	t_fdf		core;
	t_camera	camera;

	ft_bzero(&core, sizeof(t_fdf));
	if (ac != 2)
	{
		ft_printf("Usage: ./fdf [map]\n");
		return (0);
	}
	core.camera = &camera;
	core.camera->zoom = 15;
	core.camera->x = WIDTH / 2;
	core.camera->y = HEIGHT / 2;
	parse(av[1], &core);
	configure_mlx(&core.window);
	core.window.color = 0xDC143C;
	core.cur_projection = iso;
	ft_printf("%d\n", ft_len(core.matrix, 8));
	loop(&core);
	ft_printf("%d\n", ft_len(core.work, 8));

	sleep(100);
}