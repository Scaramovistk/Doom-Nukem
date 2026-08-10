/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	inside_radius(t_position a, t_position b, double radius)
{
	double	dx;
	double	dy;

	dx = a.x - b.x;
	dy = a.y - b.y;
	return (dx * dx + dy * dy < radius * radius);
}

static bool	item_blocks(t_position pos, t_game *g)
{
	int	i;

	i = -1;
	while (++i < g->map.item_count)
		if (g->map.items[i].active && g->map.items[i].blocks_passage
			&& inside_radius(pos, g->map.items[i].pos,
				ITEM_PICKUP_RADIUS + COLLISION_SAFETY))
			return (true);
	return (false);
}

static bool	world_object_blocks(t_position pos, t_game *g)
{
	double	radius;
	int		i;

	i = -1;
	while (++i < g->map.object_count)
	{
		radius = g->map.objects[i].collision_radius + COLLISION_SAFETY;
		if (g->map.objects[i].blocks_passage
			&& inside_radius(pos, g->map.objects[i].pos, radius))
			return (true);
	}
	return (false);
}

static bool	enemy_blocks(t_position pos, t_game *g)
{
	int	i;

	i = -1;
	while (++i < g->map.enemy_count)
		if (g->map.enemies[i].active && inside_radius(pos,
				g->map.enemies[i].pos, ENEMY_COLLISION_RADIUS
				+ COLLISION_SAFETY))
			return (true);
	return (false);
}

bool	is_object_blocking(t_position pos, t_game *g)
{
	return (item_blocks(pos, g) || world_object_blocks(pos, g)
		|| enemy_blocks(pos, g));
}
