/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 20:59:54 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/16 17:38:33 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_FDF_H
# define FDF_FDF_H

# include <math.h>
# include <mlx.h>
# include "../libft/includes/libft.h"
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdlib.h>

# define WIDTH		1500
# define HEIGHT		800
# define WIN_NAME	"FDF"

# define TO_RADS(angle) ((angle) * M_PI / 180.0)
# define ABS(x) (x) < 0 ? -(x) : (x)

enum			e_errors
{
	FILE_ERROR,
	MEMORY_ALLOCATION,
	MLX_ERROR
};

typedef struct	s_point
{
	double		x;
	double		y;
	double		z;
	int			color;
	int			color_set : 1;
}				t_point;

typedef struct	s_image
{
	void		*img;
	char		*matrix;
	int			bpp;
	int			endian;
	int			size_line;
}				t_image;

typedef struct	s_window
{
	void		*mlx_ptr;
	void		*win_ptr;
	unsigned	add_text_info : 1;
	int			color_id;
	t_image		img;
}				t_window;

typedef void	(*t_proj)(t_point * const);

typedef struct	s_camera
{
	int x;
	int x_angle;
	int y_angle;
	int y;
	int zoom;
}				t_camera;

typedef struct	s_button
{
	int			x;
	int			y;
	int			button_clicked : 1;
}				t_button;

typedef struct	s_fdf
{
	t_window	*window;
	t_point		**matrix;
	t_camera	*camera;
	t_proj		cur_projection;
	t_button	left;
	t_button	right;
	int			columns;
	int			rows;
}				t_fdf;

typedef struct	s_brath
{
	int			x1;
	int			y1;
	int			x2;
	int			y2;
	int			deltax;
	int			deltay;
	int			shiftx;
	int			shifty;
	int			error;
	int			error2;
}				t_brath;

void			parse(char *filename, t_fdf *core);
void			put_error(char *filename, char *add_info, int err_code);
t_point			get_work_point(t_point const *point, t_fdf *core);
void			drawline(t_point one, t_point two, t_window *win);

void			draw(t_fdf *core);

int				mouse_move(int x, int y, void *param);
int				mouse_release(int key_code, int x, int y, void *param);
int				mouse_press(int key_code, int x, int y, void *param);
int				key_press(int key_code, void *param);
int				closewin(void *core);

void			iso_projection(t_point *const original);
void			top_projection(t_point *const original);

#endif
