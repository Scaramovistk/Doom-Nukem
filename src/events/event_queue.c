/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_queue.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	queue_world_event(t_game *g, t_world_event event)
{
	int	i;

	i = 0;
	while (i < WORLD_EVENT_MAX)
	{
		if (!g->events[i].active)
		{
			g->events[i] = event;
			g->events[i].active = true;
			return ;
		}
		i++;
	}
}
