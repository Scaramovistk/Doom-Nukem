/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_sequence.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_world_event	make_event(t_event_action action, double timer,
		int value)
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
	const char		*messages[5];
	t_world_event	event;

	messages[0] = "HANGAR POWER ROUTED";
	messages[1] = "LAB LOCKDOWN OVERRIDDEN";
	messages[2] = "INFERNAL SEAL UNLATCHED";
	messages[3] = "TELEPORTER COORDINATES SET";
	messages[4] = "EARTH GATE CHARGING";
	event = make_event(EVENT_SHOW_MESSAGE, 0.0, 0);
	if (g->campaign_level >= 1 && g->campaign_level <= 5)
		ft_strlcpy(event.message, messages[g->campaign_level - 1],
			HUD_MESSAGE_LEN);
	else
		ft_strlcpy(event.message, "SWITCH ACTIVE", HUD_MESSAGE_LEN);
	queue_world_event(g, event);
	queue_world_event(g, make_event(EVENT_ADD_SCORE, 0.0, 25));
	queue_world_event(g, make_event(EVENT_TOGGLE_DOORS, SWITCH_EVENT_DELAY, 0));
	queue_world_event(g, make_event(EVENT_CLOSE_DOORS,
			SWITCH_DOOR_CLOSE_DELAY, 0));
	play_sound_effect(g, "switch");
}
