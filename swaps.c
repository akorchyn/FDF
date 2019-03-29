#include "fdf.h"

void		swap_point(t_point *one, t_point *two)
{
	t_point tmp;

	tmp = *one;
	*one = *two;
	*two = tmp;
}

void		swap_float(double *x, double *y)
{
	double tmp;

	tmp = *x;
	*x = *y;
	*y = tmp;
}