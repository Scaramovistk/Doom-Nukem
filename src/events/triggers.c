/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triggers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	in_zone(t_coord cell, t_coord *zones, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (zones[i].x == cell.x && zones[i].y == cell.y)
			return (true);
		i++;
	}
	return (false);
}

static bool	apply_hazard(t_game *g, t_coord cell)
{
	if (!in_zone(cell, g->map.hazard_zones, g->map.hazard_count))
		return (false);
	g->hud.health -= (int)(HAZARD_DAMAGE_PER_SEC * g->delta_time);
	if (g->hud.health < 0)
		g->hud.health = 0;
	return (true);
}

void	show_context_message(t_game *g)
{
	static const char	*logs[5] = {
		"UAC LOG: PHOBOS EVACUATION FAILED",
		"LAB LOG: QUARANTINE BREACH CONFIRMED",
		"HELL RELAY: THE SIGNAL COMES FROM BELOW",
		"DEIMOS CORE: TELEPORTER LOCKED",
		"EARTH GATE: STABILITY FALLING"
	};

	if (g->campaign_level >= 1 && g->campaign_level <= 5)
		show_message(g, logs[g->campaign_level - 1], MESSAGE_DISPLAY_TIME);
	else
		show_message(g, "TERMINAL OFFLINE", MESSAGE_DISPLAY_TIME);
}

static bool	apply_message(t_game *g, t_coord cell)
{
	if (in_zone(cell, g->map.message_zones, g->map.message_count))
	{
		show_context_message(g);
		return (true);
	}
	return (false);
}

bool	update_proximity_triggers(t_game *g)
{
	t_coord	cell;
	bool	hazard;
	bool	message;

	cell.x = (int)g->player.pos.x;
	cell.y = (int)g->player.pos.y;
	hazard = apply_hazard(g, cell);
	message = apply_message(g, cell);
	return (hazard || message);
}
