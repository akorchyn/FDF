#ifndef FDF_FDF_H
#define FDF_FDF_H

#include <math.h>
#include <mlx.h>
#include "libft/includes/libft.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define WIDTH		1500
#define HEIGHT		800
#define WIN_NAME	"FDF"

#define TO_RADS(angle) ((angle) * M_PI / 180.0)
#define ABS(x) (x) < 0 ? -(x) : (x)

enum			e_errors
{
	FILE_ERROR,
	MEMORY_ALLOCATION,
	MLX_ERROR
};

typedef struct	s_fdf t_fdf;

typedef struct	s_point
{
	double		x;
	double		y;
	double		z;
	int			color;
}				t_point;

typedef struct	s_image
{
	void		*img;
	char		*matrix;
	int 		bpp;
	int			endian;
	int 		size_line;
}				t_image;

typedef struct	s_window
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_image		img;
	int 		color;
}				t_window;

typedef void	(*projection)(t_point * const);

typedef struct	s_camera
{
	int x;
	int x_angle;
	int y_angle;
	int z_angle;
	int y;
	int z;
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
	projection	cur_projection;
	t_button	left;
	t_button	right;
	int 		columns;
	int 		rows;
}				t_fdf;


void			parse(char *filename, t_fdf *core);
void			put_error(char *filename, char *add_info, int err_code);
void			swap_point(t_point *one, t_point *two);
void			swap_float(double *x, double *y);
void			drawline(t_point one, t_point two, t_window *win);


#endif
