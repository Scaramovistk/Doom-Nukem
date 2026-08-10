/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_all_doors(t_game *g, bool opening)
{
	t_coord	pos;

	pos.y = 0;
	while (pos.y < g->map.height)
	{
		pos.x = 0;
		while (pos.x < g->map.width)
		{
			if (g->map.grid[pos.y][pos.x] == DOOR
				&& !g->map.doors[pos.y][pos.x].is_locked)
				g->map.doors[pos.y][pos.x].is_opening = opening;
			pos.x++;
		}
		pos.y++;
	}
}

static void	toggle_all_doors(t_game *g)
{
	t_coord	pos;

	pos.y = 0;
	while (pos.y < g->map.height)
	{
		pos.x = 0;
		while (pos.x < g->map.width)
		{
			if (g->map.grid[pos.y][pos.x] == DOOR
				&& !g->map.doors[pos.y][pos.x].is_locked)
				g->map.doors[pos.y][pos.x].is_opening
					= !g->map.doors[pos.y][pos.x].is_opening;
			pos.x++;
		}
		pos.y++;
	}
}

static void	run_event_action(t_game *g, t_world_event *event)
{
	if (event->action == EVENT_TOGGLE_DOORS)
		toggle_all_doors(g);
	else if (event->action == EVENT_OPEN_DOORS)
		set_all_doors(g, true);
	else if (event->action == EVENT_CLOSE_DOORS)
		set_all_doors(g, false);
	else if (event->action == EVENT_ADD_SCORE)
		g->hud.score += event->value;
	else if (event->action == EVENT_DAMAGE_PLAYER)
	{
		g->hud.health -= event->value;
		if (g->hud.health < 0)
			g->hud.health = 0;
	}
	else if (event->action == EVENT_SHOW_MESSAGE)
		show_message(g, event->message, MESSAGE_DISPLAY_TIME);
	else if (event->action == EVENT_OPEN_ONE_DOOR)
		open_one_door(g, event->door_target);
	else if (event->action == EVENT_AUTHORED_ACTION)
		run_authored_action(g, event->target);
}

static bool	update_one_event(t_game *g, t_world_event *event)
{
	if (!event->active)
		return (false);
	if (event->action == EVENT_SECTOR_ANIMATE)
		return (animate_sector_event(g, event));
	event->timer -= g->delta_time;
	if (event->timer > 0.0)
		return (true);
	run_event_action(g, event);
	if (event->repeat)
		event->timer = event->reload;
	else
		event->active = false;
	return (true);
}

bool	update_world_events(t_game *g)
{
	bool	active;
	int		i;

	active = false;
	i = 0;
	while (i < WORLD_EVENT_MAX)
	{
		if (update_one_event(g, &g->events[i]))
			active = true;
		i++;
	}
	return (active);
}
