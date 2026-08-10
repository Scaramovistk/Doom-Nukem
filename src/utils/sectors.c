/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sectors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_sector	default_sector(void)
{
	t_sector	sector;

	sector.floor_z = PLAYER_FLOOR_Z;
	sector.ceil_z = 1.0;
	sector.slope_x = 0.0;
	sector.slope_y = 0.0;
	sector.light = DNK_DEFAULT_LIGHT;
	sector.active = true;
	sector.elevator_raised = false;
	sector.origin_x = 0;
	sector.origin_y = 0;
	return (sector);
}

void	init_default_sectors(t_game *g)
{
	int	y;
	int	x;

	g->map.sector_count = 1;
	g->map.sectors[0] = default_sector();
	if (!g->map.width || !g->map.height || g->map.sector_grid)
		return ;
	g->map.sector_grid = calloc_s(g->map.height, sizeof(int *), g);
	y = 0;
	while (y < g->map.height)
	{
		g->map.sector_grid[y] = calloc_s(g->map.width, sizeof(int), g);
		x = 0;
		while (x < g->map.width)
			g->map.sector_grid[y][x++] = 0;
		y++;
	}
}

bool	set_sector_info(t_game *g, int id, t_sector sector)
{
	if (id < 0 || id >= SECTOR_MAX)
		return (false);
	if (sector.ceil_z <= sector.floor_z + 0.25)
		sector.ceil_z = sector.floor_z + 0.25;
	if (sector.light < 0)
		sector.light = 0;
	if (sector.light > 255)
		sector.light = 255;
	sector.active = true;
	g->map.sectors[id] = sector;
	if (id >= g->map.sector_count)
		g->map.sector_count = id + 1;
	return (true);
}

bool	set_sector_cell(t_game *g, int x, int y, int sector)
{
	if (!g->map.sector_grid || x < 0 || y < 0 || x >= g->map.width
		|| y >= g->map.height || sector < 0 || sector >= SECTOR_MAX)
		return (false);
	g->map.sector_grid[y][x] = sector;
	return (true);
}
