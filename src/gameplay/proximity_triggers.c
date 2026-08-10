/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proximity_triggers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
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
	int	damage;

	if (!in_zone(cell, g->map.hazard_zones, g->map.hazard_count))
	{
		g->hazard_damage_accumulator = 0.0;
		return (false);
	}
	g->hazard_damage_accumulator += HAZARD_DAMAGE_PER_SEC * g->delta_time;
	damage = (int)(g->hazard_damage_accumulator + HAZARD_DAMAGE_EPSILON);
	g->hazard_damage_accumulator -= damage;
	if (g->hazard_damage_accumulator < 0.0)
		g->hazard_damage_accumulator = 0.0;
	g->hud.health -= damage;
	if (g->hud.health < 0)
		g->hud.health = 0;
	return (true);
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

static bool	open_nearby_secret(t_game *g)
{
	t_coord	secret;
	double	dx;
	double	dy;
	int		i;

	i = 0;
	while (i < g->map.secret_count)
	{
		secret = g->map.secrets[i++];
		dx = secret.x + 0.5 - g->player.pos.x;
		dy = secret.y + 0.5 - g->player.pos.y;
		if (dx * dx + dy * dy <= SECRET_OPEN_RADIUS * SECRET_OPEN_RADIUS
			&& !g->map.doors[secret.y][secret.x].discovered)
		{
			trigger_secret_switch(g, secret);
			return (true);
		}
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
	return (hazard || message || open_nearby_secret(g)
		|| update_laptop_proximity(g));
}
