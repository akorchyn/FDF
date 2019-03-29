#include "libft.h"

int		ft_hex(const char *str)
{
	int result;

	if (!str)
		return (0);
	result = 0;
	while (*str)
	{
		if (ft_isdigit(*str))
			result = result * 16 + *str - '0';
		else if (*str >= 'a' && *str <= 'f')
			result = result * 16 + 10 + *str - 'a';
		else if (*str >= 'A' && *str <= 'F')
			result = result * 16 + 10 + *str - 'A';
		else
			return (0);
		str++;
	}
	return (result);
}