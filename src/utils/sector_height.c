/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_height.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_sector	*get_sector_at_cell(t_game *g, t_coord cell)
{
	int	id;

	if (!g->map.sector_count)
		init_default_sectors(g);
	id = 0;
	if (g->map.sector_grid && cell.x >= 0 && cell.y >= 0
		&& cell.x < g->map.width && cell.y < g->map.height)
		id = g->map.sector_grid[cell.y][cell.x];
	if (id < 0 || id >= SECTOR_MAX || !g->map.sectors[id].active)
		id = 0;
	return (&g->map.sectors[id]);
}

static t_coord	cell_from_pos(t_position pos)
{
	t_coord	cell;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	return (cell);
}

double	get_floor_z_at(t_game *g, t_position pos)
{
	t_sector	*sector;
	double		local_x;
	double		local_y;

	sector = get_sector_at_cell(g, cell_from_pos(pos));
	local_x = pos.x - sector->origin_x;
	local_y = pos.y - sector->origin_y;
	return (sector->floor_z + sector->slope_x * local_x + sector->slope_y
		* local_y);
}

double	get_ceiling_z_at(t_game *g, t_position pos)
{
	t_sector	*sector;
	double		ceil_z;
	double		floor_z;

	sector = get_sector_at_cell(g, cell_from_pos(pos));
	ceil_z = sector->ceil_z + sector->slope_x * (pos.x - sector->origin_x);
	ceil_z += sector->slope_y * (pos.y - sector->origin_y);
	floor_z = get_floor_z_at(g, pos);
	if (ceil_z < floor_z + 0.25)
		ceil_z = floor_z + 0.25;
	return (ceil_z);
}

double	get_floor_z_at_cell(t_game *g, t_coord cell)
{
	t_position	pos;

	pos.x = cell.x + 0.5;
	pos.y = cell.y + 0.5;
	return (get_floor_z_at(g, pos));
}
