/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_switch(t_coord pos, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.switch_count)
	{
		if (g->map.switches[i].x == pos.x && g->map.switches[i].y == pos.y)
			return (true);
		i++;
	}
	return (false);
}

static void	toggle_all_doors(t_game *g)
{
	t_coord	pos;

	pos.y = 0;
	while (pos.y < g->map.height)
	{
		pos.x = 0;
		while (pos.x < g->map.width)
		{
			if (g->map.grid[pos.y][pos.x] == DOOR)
				g->map.doors[pos.y][pos.x].is_opening
					= !g->map.doors[pos.y][pos.x].is_opening;
			pos.x++;
		}
		pos.y++;
	}
	play_sound_effect("switch");
}

static bool	try_pickup_item_at(t_coord pos, t_game *g)
{
	int		i;
	t_coord	item_cell;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active)
		{
			item_cell.x = (int)g->map.items[i].pos.x;
			item_cell.y = (int)g->map.items[i].pos.y;
			if (item_cell.x == pos.x && item_cell.y == pos.y)
			{
				collect_item(&g->map.items[i], g);
				return (true);
			}
		}
		i++;
	}
	return (false);
}

void	interact(t_game *g)
{
	t_position	dir;
	t_coord		check;
	double		check_distance;

	dir = (t_position){cos(g->player.orientation), sin(g->player.orientation)};
	check_distance = 0.0;
	while (check_distance < INTERACT_RANGE)
	{
		check.x = (int)(g->player.pos.x + dir.x * check_distance);
		check.y = (int)(g->player.pos.y + dir.y * check_distance);
		if (!is_in_bounds(check, g))
			return ;
		if (try_pickup_item_at(check, g))
			return ;
		if (is_switch(check, g))
		{
			toggle_all_doors(g);
			return ;
		}
		if (is_door(check, g) && !is_on_player(check, g))
		{
			activate_door(check, g);
			play_sound_effect("door");
			return ;
		}
		check_distance += 0.1;
	}
}
