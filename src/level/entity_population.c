/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_population.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	item_default_blocks(int type)
{
	(void)type;
	return (false);
}

void	add_items(t_map_scan *scan)
{
	int	vert;
	int	hor;

	if (!scan->g->map.item_count)
		return ;
	scan->g->map.items = calloc_s(scan->g->map.item_count, sizeof(t_item),
			scan->g);
	scan->start = scan->index;
	vert = -1;
	while (++vert < scan->lines)
	{
		hor = -1;
		while (++hor < scan->width)
		{
			if (!ft_is_item_char(scan->map[vert][hor]))
				continue ;
			add_item_cell(scan, hor, vert);
		}
	}
}

t_enemy_stats	enemy_stats_for_type(int type)
{
	if (type == 1)
		return ((t_enemy_stats){30, true, ENEMY_MOVE_SPEED, 0, 0.0, 0.0,
			ENEMY_ALERT_RANGE, ENEMY_FIRE_DELAY, ENEMY_RANGED_RANGE, 6, 25});
	if (type == 2)
		return ((t_enemy_stats){45, true, ENEMY_MOVE_SPEED, 0, 0.0, 0.0,
			ENEMY_ALERT_RANGE, 1.3, ENEMY_RANGED_RANGE, 8, 35});
	if (type == 3)
		return ((t_enemy_stats){60, false, ENEMY_MOVE_SPEED * 1.6, 12, 0.6,
			ENEMY_ATTACK_RANGE, ENEMY_ALERT_RANGE * 1.3, 0.0, 0.0, 0, 45});
	if (type == 4)
		return ((t_enemy_stats){120, true, ENEMY_MOVE_SPEED * 0.5, 0, 0.0, 0.0,
			ENEMY_ALERT_RANGE, 1.8, ENEMY_RANGED_RANGE * 1.2, 12, 60});
	return ((t_enemy_stats){20, false, ENEMY_MOVE_SPEED, 6, ENEMY_ATTACK_DELAY,
		ENEMY_ATTACK_RANGE, ENEMY_ALERT_RANGE, 0.0, 0.0, 0, 10});
}

void	add_enemies(int count, int *types, t_game *g)
{
	int				i;
	t_enemy_stats	stats;

	g->map.enemy_count = count;
	if (!count)
		return ;
	g->map.enemies = calloc_s(count, sizeof(t_enemy), g);
	i = 0;
	while (i < count)
	{
		stats = enemy_stats_for_type(types[i]);
		init_enemy(&g->map.enemies[i], stats, types[i], g->map.sprites[i]);
		g->map.enemies[i].sprite_index = i;
		i++;
	}
}

void	add_sprites(char **map, int lines, int width, t_game *g)
{
	t_sprite_counts	counts;
	t_map_scan		scan;
	int				*types;

	scan = (t_map_scan){map, lines, width, g, 0, 0};
	count_sprite_entities(&scan, &counts);
	prepare_sprite_storage(&scan, &counts);
	if (!g->map.sprite_count)
		return ;
	g->map.sprites = calloc_s(g->map.sprite_count, sizeof(t_position), g);
	types = NULL;
	if (counts.enemies)
		types = calloc_s(counts.enemies, sizeof(int), g);
	add_enemy_sprite_cells(&scan, types);
	add_enemies(counts.enemies, types, g);
	add_remaining_sprites(&scan, &counts);
}
