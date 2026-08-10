/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_sprite_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	count_sprite_entities(t_map_scan *scan, t_sprite_counts *counts)
{
	counts->enemies = count_sprites(scan->map, scan->lines, scan->width);
	counts->objects = count_world_objects(scan->map, scan->lines, scan->width);
	counts->billboards = count_billboard_decorations(scan->map, scan->lines,
			scan->width);
	counts->vending = count_vending_machines(scan->map, scan->lines,
			scan->width);
	counts->laptops = count_laptops(scan->map, scan->lines, scan->width);
	counts->flags = count_flags(scan->map, scan->lines, scan->width);
	if (counts->vending > 1)
		error("Only one vending machine is allowed per map", scan->g);
}

void	prepare_sprite_storage(t_map_scan *scan, t_sprite_counts *counts)
{
	scan->g->map.item_count = count_items(scan->map, scan->lines, scan->width);
	scan->g->map.laptop_count = counts->laptops;
	add_decorations(scan->map, scan->lines, scan->width, scan->g);
	scan->g->map.sprite_count = counts->enemies + counts->objects
		+ counts->billboards + counts->vending + counts->laptops + counts->flags
		+ scan->g->map.item_count;
}

void	add_enemy_sprite_cells(t_map_scan *scan, int *types)
{
	int	x;
	int	y;

	y = 0;
	while (y < scan->lines)
	{
		x = 0;
		while (x < scan->width)
		{
			if (ft_is_enemy_char(scan->map[y][x]))
			{
				types[scan->index] = enemy_type_from_char(scan->map[y][x]);
				scan->g->map.sprites[scan->index++]
					= (t_position){x + 0.5, y + 0.5};
			}
			x++;
		}
		y++;
	}
}

static void	add_billboard_sprites(t_map_scan *scan)
{
	int	i;

	i = 0;
	while (i < scan->g->map.decoration_count)
	{
		if (scan->g->map.decorations[i].sprite_index == -2)
		{
			scan->g->map.decorations[i].sprite_index = scan->index;
			scan->g->map.sprites[scan->index++]
				= scan->g->map.decorations[i].pos;
		}
		i++;
	}
}

void	add_remaining_sprites(t_map_scan *scan, t_sprite_counts *counts)
{
	add_world_objects(scan);
	add_billboard_sprites(scan);
	if (counts->vending)
	{
		add_vending_machine(scan);
		scan->index++;
	}
	add_laptops(scan);
	add_flag_sprite(scan);
	add_items(scan);
}
