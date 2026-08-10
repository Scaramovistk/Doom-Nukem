/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authored_target_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	block_target_valid(t_game *g, t_authored_action *a)
{
	if (!is_in_bounds(a->cell, g))
		return (false);
	if (a->value == DOOR)
		return (texture_available(g, DOOR_T));
	if (a->value == TRANSPARENT_WALL)
		return (texture_available(g, TRANSPARENT_T));
	if (a->value == DECAL_WALL)
		return (texture_available(g, DECAL_T));
	return (true);
}

bool	object_target_valid(t_game *g, t_authored_action *a)
{
	if (a->target < 0 || a->target >= g->map.object_count)
		return (false);
	return (a->type != AUTH_ACTION_OBJECT_TEXTURE
		|| texture_available(g, a->value));
}

bool	wall_target_valid(t_game *g, t_authored_action *a)
{
	if (a->target < 0 || a->target >= g->map.segment_count)
		return (false);
	return (a->type != AUTH_ACTION_WALL_TEXTURE
		|| texture_available(g, a->value));
}

bool	queue_authored_action(t_game *g, int index)
{
	t_world_event	event;

	ft_bzero(&event, sizeof(event));
	event.action = EVENT_AUTHORED_ACTION;
	event.timer = g->map.actions[index].delay;
	event.target = index;
	event.active = true;
	queue_world_event(g, event);
	return (true);
}
