/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_targets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	sector_is_moving(t_game *g, int id)
{
	int	i;

	i = 0;
	while (i < WORLD_EVENT_MAX)
	{
		if (g->events[i].active && g->events[i].action == EVENT_SECTOR_ANIMATE
			&& g->events[i].target == id)
			return (true);
		i++;
	}
	return (false);
}

void	trigger_elevator_switch(t_game *g, t_coord pos)
{
	int				id;
	t_sector		*sector;
	double			from;
	double			to;
	t_world_event	event;

	id = g->map.sector_grid[pos.y][pos.x];
	sector = &g->map.sectors[id];
	if (!sector->active)
		return (show_message(g, "ELEVATOR UNPOWERED", MESSAGE_DISPLAY_TIME));
	if (sector_is_moving(g, id))
		return (show_message(g, "ELEVATOR MOVING", 1.0));
	from = sector->floor_z;
	if (sector->elevator_raised)
		to = from - ELEVATOR_RISE;
	else
		to = from + ELEVATOR_RISE;
	sector->elevator_raised = !sector->elevator_raised;
	make_event_sector(&event, id, from, to);
	queue_world_event(g, event);
	show_message(g, "ELEVATOR ACTIVE", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "switch");
}

bool	is_secret_cell(t_game *g, t_coord pos)
{
	int	i;

	i = 0;
	while (i < g->map.secret_count)
	{
		if (g->map.secrets[i].x == pos.x && g->map.secrets[i].y == pos.y)
			return (true);
		i++;
	}
	return (false);
}

bool	is_locked_cell(t_game *g, t_coord pos)
{
	int	i;

	i = 0;
	while (i < g->map.locked_door_count)
	{
		if (g->map.locked_doors[i].x == pos.x
			&& g->map.locked_doors[i].y == pos.y)
			return (true);
		i++;
	}
	return (false);
}

void	trigger_secret_switch(t_game *g, t_coord pos)
{
	t_door	*door;

	if (!is_in_bounds(pos, g) || g->map.grid[pos.y][pos.x] != DOOR)
		return ;
	door = &g->map.doors[pos.y][pos.x];
	door->is_opening = true;
	door->discovered = true;
	show_message(g, "SECRET FOUND", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "door");
}
