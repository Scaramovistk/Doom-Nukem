/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_population.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	init_enemy(t_enemy *enemy, t_enemy_stats stats, int type,
		t_position position)
{
	enemy->pos = position;
	enemy->health = stats.health;
	enemy->max_health = stats.health;
	enemy->attack_timer = 0.0;
	enemy->fire_timer = 0.0;
	enemy->type = type;
	enemy->is_ranged = stats.is_ranged;
	enemy->active = true;
	enemy->move_speed = stats.move_speed;
	enemy->contact_damage = stats.contact_damage;
	enemy->attack_delay = stats.attack_delay;
	enemy->attack_range_sq = stats.attack_range * stats.attack_range;
	enemy->alert_range_sq = stats.alert_range * stats.alert_range;
	enemy->fire_delay = stats.fire_delay;
	enemy->ranged_range_sq = stats.ranged_range * stats.ranged_range;
	enemy->projectile_damage = stats.projectile_damage;
	enemy->score_value = stats.score_value;
}

void	add_flag_sprite(t_map_scan *scan)
{
	int	x;
	int	y;

	y = 0;
	while (y < scan->lines)
	{
		x = 0;
		while (x < scan->width)
		{
			if (scan->map[y][x] == 'G')
			{
				scan->g->map.has_flag = true;
				scan->g->map.flag_pos = (t_position){x + 0.5, y + 0.5};
				scan->g->map.flag_sprite_index = scan->index++;
				scan->g->map.sprites[scan->g->map.flag_sprite_index]
					= scan->g->map.flag_pos;
			}
			x++;
		}
		y++;
	}
}
