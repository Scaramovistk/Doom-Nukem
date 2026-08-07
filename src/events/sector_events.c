/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_events.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	make_event_sector(t_world_event *event, int target,
		double from_value, double to_value)
{
	event->action = EVENT_SECTOR_ANIMATE;
	event->timer = ELEVATOR_DURATION;
	event->reload = ELEVATOR_DURATION;
	event->value = 0;
	event->target = target;
	event->from_value = from_value;
	event->to_value = to_value;
	event->door_target = (t_coord){0, 0};
	event->repeat = false;
	event->active = true;
	event->message[0] = '\0';
}

bool	animate_sector_event(t_game *g, t_world_event *event)
{
	t_sector	*sector;
	double		ratio;

	sector = &g->map.sectors[event->target];
	event->timer -= g->delta_time;
	ratio = (event->reload - event->timer) / event->reload;
	if (ratio < 0.0)
		ratio = 0.0;
	if (ratio > 1.0)
		ratio = 1.0;
	sector->floor_z = event->from_value
		+ (event->to_value - event->from_value) * ratio;
	if (sector->floor_z > sector->ceil_z - 0.25)
		sector->floor_z = sector->ceil_z - 0.25;
	if (g->player.on_ground && g->map.sector_grid
		&& g->map.sector_grid[(int)g->player.pos.y][(int)g->player.pos.x]
		== event->target)
		g->player.z = get_floor_z_at(g, g->player.pos);
	if (ratio >= 1.0)
		event->active = false;
	return (true);
}

void	make_event_door(t_world_event *event, t_coord target, double delay)
{
	event->action = EVENT_OPEN_ONE_DOOR;
	event->timer = delay;
	event->reload = delay;
	event->value = 0;
	event->target = 0;
	event->from_value = 0.0;
	event->to_value = 0.0;
	event->door_target = target;
	event->repeat = false;
	event->active = true;
	event->message[0] = '\0';
}
