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

void	toggle_door(t_game *g)
{
	t_position	dir;
	t_coord		check;
	double		check_distance;
	bool		open;

	dir = (t_position){cos(g->player.orientation), sin(g->player.orientation)};
	check_distance = 0.0;
	while (check_distance < 2.5)
	{
		check.x = (int)(g->player.pos.x + dir.x * check_distance);
		check.y = (int)(g->player.pos.y + dir.y * check_distance);
		open = (is_in_bounds(check, g) && is_door(check, g));
		if (open && !is_on_player(check, g))
		{
			activate_door(check, g);
			break ;
		}
		check_distance += 0.1;
	}
}

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
