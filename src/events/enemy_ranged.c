/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ranged.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	fire_enemy_projectile(t_game *g, t_enemy *enemy)
{
	t_shot_spec	spec;
	double		angle;

	angle = atan2(g->player.pos.y - enemy->pos.y,
			g->player.pos.x - enemy->pos.x);
	spec = (t_shot_spec){enemy->pos, angle, enemy->projectile_damage, true};
	fire_projectile_from(g, spec);
}

bool	enemy_ranged_attack(t_enemy *enemy, t_game *g, double distance)
{
	double	shot_z;

	if (distance > enemy->ranged_range_sq)
		return (false);
	shot_z = get_floor_z_at(g, enemy->pos) + PLAYER_STAND_HEIGHT;
	if (projectile_path_is_blocked(g, enemy->pos, g->player.pos, shot_z))
		return (false);
	enemy->fire_timer -= g->delta_time;
	if (enemy->fire_timer <= 0.0)
	{
		fire_enemy_projectile(g, enemy);
		enemy->fire_timer = enemy->fire_delay;
		return (true);
	}
	return (false);
}
