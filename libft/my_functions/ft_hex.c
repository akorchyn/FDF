/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 17:46:22 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/16 17:46:22 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
