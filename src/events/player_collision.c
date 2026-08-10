/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_coord	get_adjacent_cell(t_position pos, const t_position add)
{
	t_coord	cell;

	cell.x = (int)(pos.x + add.x * COLLISION_SAFETY);
	cell.y = (int)(pos.y + add.y * COLLISION_SAFETY);
	return (cell);
}

bool	is_door_open(t_coord cell, t_door **doors)
{
	if (doors[cell.y][cell.x].opening_state == 1.0)
		return (true);
	return (false);
}

static bool	height_allows_position(t_position pos, t_game *g)
{
	double	current_floor;
	double	target_floor;

	current_floor = get_floor_z_at(g, g->player.pos);
	target_floor = get_floor_z_at(g, pos);
	if (!player_has_free_movement(&g->player) && target_floor > current_floor
		+ PLAYER_STEP_HEIGHT)
		return (false);
	if (player_has_free_movement(&g->player) && target_floor > g->player.z
		+ PLAYER_STEP_HEIGHT)
		return (false);
	if (player_has_free_movement(&g->player) && g->player.z
		+ g->player.eye_height > get_ceiling_z_at(g, pos))
		return (false);
	if (!player_has_free_movement(&g->player) && get_ceiling_z_at(g, pos)
		- target_floor < g->player.eye_height)
		return (false);
	return (true);
}

static bool	adjacent_cells_allow(t_position pos, t_game *g)
{
	const t_position	checks[4] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
	t_coord				cell;
	t_block				block;
	int					i;

	i = 0;
	while (i < 4)
	{
		cell = get_adjacent_cell(pos, checks[i]);
		block = g->map.grid[cell.y][cell.x];
		if (block == WALL || block == TRANSPARENT_WALL || block == DECAL_WALL)
			return (false);
		if (block == DOOR && !is_door_open(cell, g->map.doors))
			return (false);
		i++;
	}
	return (true);
}

bool	is_position_legal(t_position pos, t_game *g)
{
	if (segment_blocks_position(g, pos))
		return (false);
	if (is_object_blocking(pos, g))
		return (false);
	if (!height_allows_position(pos, g))
		return (false);
	return (adjacent_cells_allow(pos, g));
}
