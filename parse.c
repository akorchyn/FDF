#include "fdf.h"

static int 		open_file(char *filename)
{
	int		fd;
	char	*extension;

	extension = strrchr(filename, '.');
	if (!extension || ft_strcmp(extension, ".fdf"))
		put_error(filename, "Bad extension", FILE_ERROR);
	if ((fd = open(filename, O_RDONLY)) == -1 || read(fd, NULL, 0) == -1)
		put_error(filename, strerror(errno), FILE_ERROR);
	return fd;
}

static t_point	**get_new_pointer(t_point **x)
{
	int		size;
	t_point **new;
	int		i;

	i = -1;
	size = x ? ft_len(x, sizeof(t_point **)) : 0;
	if (!(new = (t_point **)ft_memalloc(sizeof(t_point *) * (size + 2))))
		put_error(NULL, "Can't allocate memory", MEMORY_ALLOCATION);
	while (++i < size)
		new[i] = x[i];
	free(x);
	return (new);
}

static t_point	*get_points(char **points, t_fdf *core, int row)
{
	int		size;
	t_point	*result;
	char 	**numbers;
	int 	i;

	size = ft_len(points, sizeof(char *));
	i = -1;
	core->columns == 0 && (core->columns = size);
	if (size != core->columns)
		put_error(NULL, "File has different amount of points", FILE_ERROR);
	if (!(result = (t_point *)ft_memalloc(sizeof(t_point) * (size + 1))))
		put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
	while (++i < size)
	{
		if (!(numbers = ft_strsplit(points[i], ',')))
			put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
		result[i].x = i;
		result[i].y = row;
		result[i].z = ft_atoi(numbers[0]);
		result[i].color = (numbers[1]) ? ft_hex(numbers[1] + 2) : 0;
		ft_freesplit(numbers);
	}
	return result;
}

void		parse(char *filename, t_fdf *core)
{
	int		fd;
	char	**points;
	int 	y;
	char	*line;

	y = 0;
	fd = open_file(filename);
	while (get_next_line(fd, &line))
	{
		core->matrix = get_new_pointer(core->matrix);
		if (!(points = ft_strsplit(line, ' ')))
			put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
		core->matrix[y] = get_points(points, core, y);
		y++;
		ft_freesplit(points);
	}
	core->rows = y;
	close(fd);
}