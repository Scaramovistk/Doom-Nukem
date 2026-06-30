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
			if (g->map.grid[pos.y][pos.x] == DOOR)
			{
				g->map.doors[pos.y][pos.x].opening_state = 0.0;
				g->map.doors[pos.y][pos.x].is_opening = false;
			}
			else
			{
				g->map.doors[pos.y][pos.x].opening_state = -1.0;
				g->map.doors[pos.y][pos.x].is_opening = false;
			}
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
			if (g->map.grid[pos.y][pos.x] == DOOR)
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
