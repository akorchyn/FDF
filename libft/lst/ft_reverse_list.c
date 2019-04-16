/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reverse_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:44:10 by akorchyn          #+#    #+#             */
/*   Updated: 2019/04/16 16:59:09 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_reverse_list(void **lst)
{
	void	**fast;
	void	**current;
	void	**prev;

	if (!lst || !*lst)
		return ;
	current = (void **)*lst;
	prev = NULL;
	while (current)
	{
		fast = current[0];
		current[0] = prev;
		prev = current;
		current = fast;
	}
	*lst = prev;
}
