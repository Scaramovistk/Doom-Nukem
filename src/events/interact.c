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

static bool	in_coord_list(t_coord pos, t_coord *list, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (list[i].x == pos.x && list[i].y == pos.y)
			return (true);
		i++;
	}
	return (false);
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

static bool	try_switches_at(t_coord check, t_game *g)
{
	if (in_coord_list(check, g->map.switches, g->map.switch_count))
	{
		trigger_switch_sequence(g);
		return (true);
	}
	if (in_coord_list(check, g->map.elevators, g->map.elevator_count))
	{
		trigger_elevator_switch(g, check);
		return (true);
	}
	if (in_coord_list(check, g->map.secrets, g->map.secret_count))
	{
		if (consume_key(g))
			trigger_secret_switch(g, check);
		else
			show_message(g, "NEED A KEY", MESSAGE_DISPLAY_TIME);
		return (true);
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
		if (try_switches_at(check, g))
			return ;
		if (is_door(check, g) && !is_on_player(check, g))
		{
			activate_door(check, g);
			play_sound_effect(g, "door");
			return ;
		}
		check_distance += 0.1;
	}
}
