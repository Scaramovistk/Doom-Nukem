/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
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
			if (g->map.grid[pos.y][pos.x] == DOOR)
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
			if (g->map.grid[pos.y][pos.x] == DOOR)
				g->map.doors[pos.y][pos.x].is_opening
					= !g->map.doors[pos.y][pos.x].is_opening;
			pos.x++;
		}
		pos.y++;
	}
}

static void	show_event_message(t_game *g, t_world_event *event)
{
	show_message(g, event->message, MESSAGE_DISPLAY_TIME);
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
		show_event_message(g, event);
}

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

static bool	update_one_event(t_game *g, t_world_event *event)
{
	if (!event->active)
		return (false);
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

static t_world_event	make_event(t_event_action action, double timer, int value)
{
	t_world_event	event;

	event.action = action;
	event.timer = timer;
	event.reload = timer;
	event.value = value;
	event.repeat = false;
	event.active = true;
	event.message[0] = '\0';
	return (event);
}

void	trigger_switch_sequence(t_game *g)
{
	t_world_event	event;

	event = make_event(EVENT_SHOW_MESSAGE, 0.0, 0);
	ft_strlcpy(event.message, "SWITCH ACTIVE", HUD_MESSAGE_LEN);
	queue_world_event(g, event);
	queue_world_event(g, make_event(EVENT_ADD_SCORE, 0.0, 25));
	queue_world_event(g, make_event(EVENT_TOGGLE_DOORS, SWITCH_EVENT_DELAY, 0));
	queue_world_event(g, make_event(EVENT_CLOSE_DOORS,
			SWITCH_DOOR_CLOSE_DELAY, 0));
	play_sound_effect(g, "switch");
}
