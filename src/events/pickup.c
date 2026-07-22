/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	in_pickup_range(t_position a, t_position b)
{
	double	dx;
	double	dy;

	dx = a.x - b.x;
	dy = a.y - b.y;
	return (dx * dx + dy * dy <= ITEM_PICKUP_RADIUS * ITEM_PICKUP_RADIUS);
}

static void	relink_moved_item(t_game *g, int old_index, int new_index)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].sprite_index == old_index)
		{
			g->map.items[i].sprite_index = new_index;
			return ;
		}
		i++;
	}
}

static void	remove_item_sprite(t_item *item, t_game *g)
{
	int	last;

	last = g->map.sprite_count - 1;
	if (item->sprite_index != last)
	{
		g->map.sprites[item->sprite_index] = g->map.sprites[last];
		relink_moved_item(g, last, item->sprite_index);
	}
	g->map.sprite_count--;
	item->sprite_index = -1;
}

void	collect_item(t_item *item, t_game *g)
{
	g->hud.inventory[item->type] += item->quantity;
	item->active = false;
	remove_item_sprite(item, g);
	show_message(g, "ITEM PICKED UP", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}

void	update_item_pickups(t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active
			&& in_pickup_range(g->player.pos, g->map.items[i].pos))
			collect_item(&g->map.items[i], g);
		i++;
	}
}

bool	is_item_blocking(t_coord cell, t_game *g)
{
	int		i;
	t_coord	item_cell;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active && g->map.items[i].blocks_passage)
		{
			item_cell.x = (int)g->map.items[i].pos.x;
			item_cell.y = (int)g->map.items[i].pos.y;
			if (item_cell.x == cell.x && item_cell.y == cell.y)
				return (true);
		}
		i++;
	}
	return (false);
}
