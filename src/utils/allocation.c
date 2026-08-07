/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:19:17 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:19:20 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	*s_alloc(void *pointer, t_game *g)
{
	if (!pointer)
		error("Memory allocation", g);
	track_alloc(pointer, g);
	return (pointer);
}

void	*calloc_s(size_t count, size_t size, t_game *g)
{
	return (s_alloc(ft_calloc(count, size), g));
}

void	track_alloc(void *pointer, t_game *g)
{
	t_list	*new_node;

	new_node = ft_lstnew(pointer);
	if (!new_node)
	{
		free(pointer);
		error("Memory allocation", g);
	}
	ft_lstadd_front(&g->allocated_pointers, new_node);
}
