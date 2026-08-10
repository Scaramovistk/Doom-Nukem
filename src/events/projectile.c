/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_position	path_point(t_position start, t_position delta,
		double progress, double length)
{
	t_position	pos;

	pos.x = start.x + delta.x * progress / length;
	pos.y = start.y + delta.y * progress / length;
	return (pos);
}

static bool	projectile_cell_is_blocked(t_game *g, t_position pos, double z)
{
	t_coord	cell;
	t_block	block;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (!is_in_bounds(cell, g))
		return (true);
	block = g->map.grid[cell.y][cell.x];
	if (block == WALL || block == DECAL_WALL || block == TRANSPARENT_WALL)
		return (true);
	if (block == DOOR && !is_door_open(cell, g->map.doors))
		return (true);
	return (z <= get_floor_z_at(g, pos) + 0.01 || z >= get_ceiling_z_at(g, pos)
		- 0.01);
}

bool	projectile_path_is_blocked(t_game *g, t_position start, t_position end,
		double z)
{
	t_position	pos;
	t_position	delta;
	double		length;
	double		progress;

	delta.x = end.x - start.x;
	delta.y = end.y - start.y;
	length = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (length < 0.001)
		return (projectile_cell_is_blocked(g, end, z));
	progress = 0.0;
	while (progress <= length)
	{
		pos = path_point(start, delta, progress, length);
		if (projectile_cell_is_blocked(g, pos, z))
			return (true);
		progress += 0.05;
	}
	return (projectile_cell_is_blocked(g, end, z));
}

bool	hit_wall(t_game *g, t_projectile *p, t_position pos)
{
	t_coord	cell;

	if (!projectile_path_is_blocked(g, p->pos, pos, p->z))
		return (false);
	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (is_in_bounds(cell, g) && g->map.grid[cell.y][cell.x] == WALL)
		g->map.grid[cell.y][cell.x] = DECAL_WALL;
	return (true);
}
