/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoration_population.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	button_neighbor(char c)
{
	return (c != '1' && c != '4' && c != '5' && c != 'L' && c != ' ');
}

t_position	button_position(t_map_scan *scan, int x, int y)
{
	t_position	pos;

	pos = (t_position){x + 0.5, y + 0.5};
	if (y > 0 && button_neighbor(scan->map[y - 1][x]))
		pos.y -= ELEVATOR_BUTTON_OFFSET;
	else if (y + 1 < scan->lines && button_neighbor(scan->map[y + 1][x]))
		pos.y += ELEVATOR_BUTTON_OFFSET;
	else if (x > 0 && button_neighbor(scan->map[y][x - 1]))
		pos.x -= ELEVATOR_BUTTON_OFFSET;
	else if (x + 1 < scan->width && button_neighbor(scan->map[y][x + 1]))
		pos.x += ELEVATOR_BUTTON_OFFSET;
	return (pos);
}

void	add_decorations(char **map, int lines, int width, t_game *g)
{
	int			x;
	int			y;
	t_map_scan	scan;

	scan = (t_map_scan){map, lines, width, g, 0, 0};
	g->map.decoration_count = count_decorations(map, lines, width);
	if (!g->map.decoration_count)
		return ;
	g->map.decorations = calloc_s(g->map.decoration_count, sizeof(t_decoration),
			g);
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
		{
			add_decoration_cell(&scan, x, y);
			x++;
		}
		y++;
	}
}

int	count_world_objects(char **map, int lines, int width)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
		{
			count += (map[y][x] == 'V' || map[y][x] == 'v');
			x++;
		}
		y++;
	}
	return (count);
}

void	add_world_objects(t_map_scan *scan)
{
	int	x;
	int	y;

	scan->g->map.object_count = count_world_objects(scan->map, scan->lines,
			scan->width);
	if (!scan->g->map.object_count)
		return ;
	scan->g->map.objects = calloc_s(scan->g->map.object_count,
			sizeof(t_world_object), scan->g);
	scan->start = scan->index;
	y = 0;
	while (y < scan->lines)
	{
		x = 0;
		while (x < scan->width)
		{
			if (scan->map[y][x] == 'V' || scan->map[y][x] == 'v')
				add_world_object_cell(scan, x, y);
			x++;
		}
		y++;
	}
}
