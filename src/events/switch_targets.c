/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_targets.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	trigger_elevator_switch(t_game *g, t_coord pos)
{
	int				id;
	t_sector		*sector;
	double			from;
	double			to;
	t_world_event	event;

	id = g->map.sector_grid[pos.y][pos.x];
	sector = &g->map.sectors[id];
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

static long	door_distance(t_coord door, t_coord pos)
{
	long	dx;
	long	dy;

	dx = door.x - pos.x;
	dy = door.y - pos.y;
	return (dx * dx + dy * dy);
}

static void	check_door_cell(t_game *g, t_coord cell, t_door_search *search)
{
	long	dist;

	if (g->map.grid[cell.y][cell.x] != DOOR)
		return ;
	dist = door_distance(cell, search->pos);
	if (search->best_dist < 0 || dist < search->best_dist)
	{
		search->best_dist = dist;
		search->best = cell;
	}
}

static t_coord	find_nearest_door(t_game *g, t_coord pos)
{
	t_door_search	search;
	t_coord			cell;

	search.pos = pos;
	search.best = pos;
	search.best_dist = -1;
	cell.y = 0;
	while (cell.y < g->map.height)
	{
		cell.x = 0;
		while (cell.x < g->map.width)
		{
			check_door_cell(g, cell, &search);
			cell.x++;
		}
		cell.y++;
	}
	return (search.best);
}

void	trigger_secret_switch(t_game *g, t_coord pos)
{
	t_coord			door;
	t_world_event	event;

	door = find_nearest_door(g, pos);
	make_event_door(&event, door, SWITCH_EVENT_DELAY);
	queue_world_event(g, event);
	show_message(g, "SECRET FOUND", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "switch");
}
