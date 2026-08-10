/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:11:58 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:01 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	init_door_cell(t_game *g, t_coord pos)
{
	t_door	*door;

	door = &g->map.doors[pos.y][pos.x];
	door->is_opening = false;
	door->discovered = false;
	if (g->map.grid[pos.y][pos.x] == DOOR)
	{
		door->opening_state = 0.0;
		door->is_secret = is_secret_cell(g, pos);
		door->is_locked = is_locked_cell(g, pos);
	}
	else
	{
		door->opening_state = -1.0;
		door->is_secret = false;
		door->is_locked = false;
	}
}

void	init_door_grid(t_game *g)
{
	t_coord	pos;

	g->map.doors = calloc_s(g->map.height, sizeof(t_door *), g);
	pos.y = 0;
	while (pos.y < g->map.height)
	{
		g->map.doors[pos.y] = calloc_s(g->map.width, sizeof(t_door), g);
		pos.x = 0;
		while (pos.x < g->map.width)
		{
			init_door_cell(g, pos);
			pos.x++;
		}
		pos.y++;
	}
}

void	update_doors(bool *door_updated, t_game *g)
{
	t_coord	pos;

	*door_updated = false;
	pos.y = 0;
	while (pos.y < g->map.height)
	{
		pos.x = 0;
		while (pos.x < g->map.width)
		{
			if (g->map.grid[pos.y][pos.x] == DOOR && bsp_tile_visible(g, pos))
				if (update_one_door(&g->map.doors[pos.y][pos.x], g->delta_time))
					*door_updated = true;
			pos.x++;
		}
		pos.y++;
	}
}

bool	update_one_door(t_door *door, double delta_time)
{
	bool	updated;

	updated = false;
	if (door->is_opening && door->opening_state < 1.0)
	{
		door->opening_state += DOOR_ANIMATION_SPEED * delta_time;
		if (door->opening_state > 1.0)
			door->opening_state = 1.0;
		updated = true;
	}
	else if (!door->is_opening && door->opening_state > 0.0)
	{
		door->opening_state -= DOOR_ANIMATION_SPEED * delta_time;
		if (door->opening_state < 0.0)
			door->opening_state = 0.0;
		updated = true;
	}
	return (updated);
}

void	open_one_door(t_game *g, t_coord target)
{
	if (target.x < 0 || target.y < 0 || target.x >= g->map.width
		|| target.y >= g->map.height)
		return ;
	if (g->map.grid[target.y][target.x] != DOOR)
		return ;
	if (!g->map.doors[target.y][target.x].is_locked)
		g->map.doors[target.y][target.x].is_opening = true;
}
