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

static bool	apply_message(t_game *g, t_coord cell)
{
	if (in_zone(cell, g->map.message_zones, g->map.message_count))
	{
		ft_strlcpy(g->message.text, "MESSAGE ZONE", HUD_MESSAGE_LEN);
		g->message.timer = MESSAGE_DISPLAY_TIME;
		return (true);
	}
	if (g->message.timer > 0.0)
	{
		g->message.timer -= g->delta_time;
		if (g->message.timer < 0.0)
			g->message.timer = 0.0;
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
