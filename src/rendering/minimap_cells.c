/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_cells.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	minimap_pixel(t_game *g, int x, int y, int color)
{
	if (x >= 0 && y >= 0 && x < WIN_WIDTH && y < WIN_HEIGHT)
		put_pixel(&g->img, x, y, color);
}

void	minimap_rect(t_game *g, t_coord pos, t_coord size, int color)
{
	int	x;
	int	y;

	y = pos.y;
	while (y < pos.y + size.y)
	{
		x = pos.x;
		while (x < pos.x + size.x)
			minimap_pixel(g, x++, y, color);
		y++;
	}
}

static int	block_color(t_block block)
{
	if (block == WALL)
		return (GREY);
	if (block == DOOR)
		return (ORANGE);
	if (block == TRANSPARENT_WALL)
		return (BLUE);
	if (block == DECAL_WALL)
		return (YELLOW);
	return (DARK_GREY);
}

static int	map_cell_color(t_game *g, t_coord cell)
{
	if (is_secret_cell(g, cell))
		return (GREY);
	return (block_color(g->map.grid[cell.y][cell.x]));
}

void	draw_map_cells(t_game *g, t_coord origin)
{
	t_coord	cell;
	t_coord	pos;

	cell.y = -1;
	while (++cell.y < g->map.height)
	{
		cell.x = -1;
		while (++cell.x < g->map.width)
		{
			pos.x = origin.x + cell.x * MAP_SCALE;
			pos.y = origin.y + cell.y * MAP_SCALE;
			minimap_rect(g, pos, (t_coord){MAP_SCALE - 1, MAP_SCALE - 1},
				map_cell_color(g, cell));
		}
	}
}
