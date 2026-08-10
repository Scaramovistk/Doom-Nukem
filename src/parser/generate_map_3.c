/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	count_vending_machines(char **map, int lines, int width)
{
	return (count_map_char(map, lines, width, 'Q'));
}

bool	add_vending_machine(t_map_scan *scan)
{
	int	x;
	int	y;

	y = 0;
	while (y < scan->lines)
	{
		x = 0;
		while (x < scan->width)
		{
			if (scan->map[y][x] == 'Q')
			{
				scan->g->map.vending_machine.pos
					= (t_position){x + 0.5, y + 0.5};
				scan->g->map.vending_machine.sprite_index = scan->index;
				scan->g->map.vending_machine.active = true;
				scan->g->map.sprites[scan->index]
					= scan->g->map.vending_machine.pos;
				return (true);
			}
			x++;
		}
		y++;
	}
	return (false);
}

int	count_laptops(char **map, int lines, int width)
{
	return (count_map_char(map, lines, width, 'J'));
}

void	add_laptops(t_map_scan *scan)
{
	int	x;
	int	y;

	if (!scan->g->map.laptop_count)
		return ;
	scan->g->map.laptops = calloc_s(scan->g->map.laptop_count,
			sizeof(t_laptop), scan->g);
	scan->start = scan->index;
	y = 0;
	while (y < scan->lines)
	{
		x = 0;
		while (x < scan->width)
		{
			if (scan->map[y][x] == 'J')
				add_laptop_cell(scan, x, y);
			x++;
		}
		y++;
	}
}

int	enemy_type_from_char(char c)
{
	if (c == 'K')
		return (1);
	if (c == 'I')
		return (2);
	if (c == 'D')
		return (3);
	if (c == 'C')
		return (4);
	return (0);
}
