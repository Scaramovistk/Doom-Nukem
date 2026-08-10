/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
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

void	collect_item(t_item *item, t_game *g)
{
	item->active = false;
	remove_item_sprite(item, g);
	if (item->type == ITEM_HEALTH)
		apply_health_pickup(g, item->quantity);
	else if (item->type == ITEM_AMMO)
		apply_ammo_pickup(g, item->quantity);
	else
	{
		g->hud.inventory[item->type] += item->quantity;
		show_message(g, "ITEM PICKED UP", MESSAGE_DISPLAY_TIME);
		play_sound_effect(g, "pickup");
	}
}

void	update_item_pickups(t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active && in_pickup_range(g->player.pos,
				g->map.items[i].pos))
			collect_item(&g->map.items[i], g);
		i++;
	}
}
