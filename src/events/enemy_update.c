/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static double	enemy_dist_sq(t_enemy *enemy, t_game *g)
{
	double	dx;
	double	dy;

	dx = enemy->pos.x - g->player.pos.x;
	dy = enemy->pos.y - g->player.pos.y;
	return (dx * dx + dy * dy);
}

static bool	update_one_enemy(t_enemy *enemy, t_game *g)
{
	double	distance;

	distance = enemy_dist_sq(enemy, g);
	if (enemy->is_ranged)
	{
		if (enemy_ranged_attack(enemy, g, distance))
			return (true);
		return (enemy_chase(enemy, g, distance));
	}
	if (enemy_attack(enemy, g, distance))
		return (true);
	return (enemy_chase(enemy, g, distance));
}

bool	update_enemies(t_game *g)
{
	bool	updated;
	int		i;

	updated = false;
	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active && bsp_position_visible(g,
				g->map.enemies[i].pos) && update_one_enemy(&g->map.enemies[i],
				g))
			updated = true;
		i++;
	}
	return (updated);
}
