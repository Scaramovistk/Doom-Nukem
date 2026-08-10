/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authored_actions_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	trigger_authored_actions(t_game *g, t_coord trigger)
{
	int				i;
	bool			queued;

	queued = false;
	i = 0;
	while (i < g->map.action_count)
	{
		if (g->map.actions[i].trigger.x == trigger.x
			&& g->map.actions[i].trigger.y == trigger.y)
			queued = queue_authored_action(g, i);
		i++;
	}
	if (queued)
	{
		show_message(g, "AUTHORED SEQUENCE ACTIVE", MESSAGE_DISPLAY_TIME);
		play_sound_effect(g, "switch");
	}
	return (queued);
}
