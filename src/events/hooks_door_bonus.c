/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_door_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:10 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:12 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	is_in_bounds(t_coord pos, t_game *g)
{
	bool	x_ok;

	x_ok = (pos.x >= 0 && pos.x < g->map.width);
	return (x_ok && pos.y >= 0 && pos.y < g->map.height);
}

bool	is_door(t_coord pos, t_game *g)
{
	return (g->map.grid[pos.y][pos.x] == DOOR);
}

bool	is_on_player(t_coord pos, t_game *g)
{
	return (pos.x == (int)g->player.pos.x && pos.y == (int)g->player.pos.y);
}

void	activate_door(t_coord pos, t_game *g)
{
	t_door	*door;

	door = &g->map.doors[pos.y][pos.x];
	door->is_opening = !door->is_opening;
}
