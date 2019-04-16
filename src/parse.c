/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 23:17:43 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/16 17:30:04 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int		open_file(char *filename)
{
	int		fd;
	char	*extension;

	extension = strrchr(filename, '.');
	if (!extension || ft_strcmp(extension, ".fdf"))
		put_error(filename, "Bad extension", FILE_ERROR);
	if ((fd = open(filename, O_RDONLY)) == -1 || read(fd, NULL, 0) == -1)
		put_error(filename, strerror(errno), FILE_ERROR);
	return (fd);
}

static int		bad_line(char *tmp)
{
	int		(*check_valid)(int);

	check_valid = ft_isdigit;
	if (!tmp)
		return 1;
	while (*tmp)
	{
		while (*tmp && check_valid(*tmp))
			tmp++;
		if (*tmp == ' ' || *tmp == '-' || *tmp == '\0')
		{
			check_valid = ft_isdigit;
			(*tmp) && tmp++;
		}
		else if (*tmp == ',')
		{
			tmp += 3;
			check_valid = ft_isxdigit;
		}
		else
			return (1);
	}
	return (0);
}

static t_point	*get_points(char **points, t_fdf *core, int row)
{
	int		size;
	t_point	*result;
	char	**numbers;
	int		i;

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
		result[i].color = (numbers[1]) ? ft_hex(numbers[1] + 2) : 0xFFFFFF;
		numbers[1] && (result[i].color_set = 1);
		ft_freesplit(numbers);
	}
	return (result);
}

t_list			*get_file(char *filename)
{
	t_list	*head;
	t_list	*tmp;
	int		ret;
	int		fd;
	char	*line;

	head = NULL;
	fd = open_file(filename);
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (bad_line(line))
			put_error(filename, "Map error", FILE_ERROR);
		tmp = ft_lstnew(NULL, 0);
		tmp->content = line;
		tmp->next = head;
		head = tmp;
	}
	if (ret == -1)
		put_error(filename, "Error file reading", FILE_ERROR);
	close(fd);
	ft_reverse_list((void **)&head);
	return (head);
}

void			parse(char *filename, t_fdf *core)
{
	char	**points;
	int		i;
	t_list	*file;
	t_list	*tmp;

	i = 0;
	file = get_file(filename);
	if (!file)
		put_error(NULL, "Empty file", FILE_ERROR);
	core->rows = ft_list_counter((void **)file);
	if (!(core->matrix =
			(t_point **)ft_memalloc(sizeof(t_point *) * (core->rows + 1))))
		put_error(NULL, "Can't allocate memory", MEMORY_ALLOCATION);
	while (file)
	{
		tmp = file->next;
		if (!(points = ft_strsplit(file->content, ' ')))
			put_error(NULL, "Memory allocation failed", MEMORY_ALLOCATION);
		core->matrix[i] = get_points(points, core, i);
		i++;
		ft_freesplit(points);
		free(file->content);
		free(file);
		file = tmp;
	}
}
