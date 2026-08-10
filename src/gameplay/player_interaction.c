/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_interaction.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
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
	if (in_coord_list(check, g->map.message_zones, g->map.message_count))
	{
		show_context_message(g);
		play_sound_effect(g, "switch");
		return (true);
	}
	if (in_coord_list(check, g->map.switches, g->map.switch_count))
	{
		if (!trigger_authored_actions(g, check))
			trigger_switch_sequence(g);
		return (true);
	}
	if (in_coord_list(check, g->map.elevators, g->map.elevator_count))
	{
		trigger_elevator_switch(g, check);
		return (true);
	}
	return (false);
}

static bool	use_door(t_coord pos, t_game *g)
{
	t_door	*door;

	if (!is_door(pos, g) || is_secret_cell(g, pos) || is_on_player(pos, g))
		return (false);
	door = &g->map.doors[pos.y][pos.x];
	if (door->is_locked && g->hud.inventory[ITEM_KEY] <= 0)
	{
		show_message(g, "LOCKED - KEY REQUIRED", MESSAGE_DISPLAY_TIME);
		return (true);
	}
	if (door->is_locked)
	{
		g->hud.inventory[ITEM_KEY]--;
		door->is_locked = false;
		show_message(g, "KEY USED - DOOR UNLOCKED", MESSAGE_DISPLAY_TIME);
	}
	activate_door(pos, g);
	play_sound_effect(g, "door");
	return (true);
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
		if (try_pickup_item_at(check, g)
			|| try_use_vending_machine_at(check, g)
			|| try_switches_at(check, g))
			return ;
		if (use_door(check, g))
			return ;
		if (is_door(check, g) || g->map.grid[check.y][check.x] == WALL
			|| g->map.grid[check.y][check.x] == TRANSPARENT_WALL
			|| g->map.grid[check.y][check.x] == DECAL_WALL)
			return ;
		check_distance += 0.1;
	}
}
