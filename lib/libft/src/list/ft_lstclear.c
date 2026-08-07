/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:09:59 by gscarama          #+#    #+#             */
/*   Updated: 2023/10/19 18:27:42 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*buffer;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		buffer = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = buffer;
	}
	*lst = NULL;
}
