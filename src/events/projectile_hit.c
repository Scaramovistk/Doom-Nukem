/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_hit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	sprite_in_hit_range(t_position sprite, t_position pos)
{
	double	dx;
	double	dy;

	dx = sprite.x - pos.x;
	dy = sprite.y - pos.y;
	return (dx * dx + dy * dy <= PROJECTILE_HIT_RADIUS
		* PROJECTILE_HIT_RADIUS);
}

bool	hit_sprite(t_game *g, t_projectile *p, t_position pos)
{
	int	i;

	if (p->from_enemy)
		return (false);
	i = -1;
	while (++i < g->map.sprite_count)
	{
		if (!sprite_in_hit_range(g->map.sprites[i], pos)
			|| projectile_is_decoration(g, i))
			continue ;
		if (damage_enemy_at_sprite(g, i, p->damage))
			return (true);
		remove_projectile_target(g, i, p->damage);
		return (true);
	}
	return (false);
}
