/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_origin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/24 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	mark_sector_min(t_game *g, int mins[SECTOR_MAX][2], t_coord cell)
{
	int	id;

	id = g->map.sector_grid[cell.y][cell.x];
	if (id < 0 || id >= SECTOR_MAX)
		return ;
	if (mins[id][0] < 0 || cell.x < mins[id][0])
		mins[id][0] = cell.x;
	if (mins[id][1] < 0 || cell.y < mins[id][1])
		mins[id][1] = cell.y;
}

static void	scan_sector_mins(t_game *g, int mins[SECTOR_MAX][2])
{
	t_coord	cell;

	cell.y = -1;
	while (++cell.y < g->map.height)
	{
		cell.x = -1;
		while (++cell.x < g->map.width)
			mark_sector_min(g, mins, cell);
	}
}

void	compute_sector_origins(t_game *g)
{
	int	mins[SECTOR_MAX][2];
	int	id;

	if (!g->map.sector_grid)
		return ;
	id = -1;
	while (++id < SECTOR_MAX)
	{
		mins[id][0] = -1;
		mins[id][1] = -1;
	}
	scan_sector_mins(g, mins);
	id = -1;
	while (++id < SECTOR_MAX)
	{
		if (mins[id][0] >= 0)
			g->map.sectors[id].origin_x = mins[id][0];
		if (mins[id][1] >= 0)
			g->map.sectors[id].origin_y = mins[id][1];
	}
}
