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
			g->map.items[i].sprite_index = new_index;
		i++;
	}
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == old_index)
		g->map.vending_machine.sprite_index = new_index;
	if (g->map.has_flag && g->map.flag_sprite_index == old_index)
		g->map.flag_sprite_index = new_index;
	i = 0;
	while (i < g->map.laptop_count)
	{
		if (g->map.laptops[i].sprite_index == old_index)
			g->map.laptops[i].sprite_index = new_index;
		i++;
	}
	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == old_index)
		{
			g->map.decorations[i].sprite_index = new_index;
			return ;
		}
		i++;
	}
	i = 0;
	while (i < g->map.object_count)
	{
		if (g->map.objects[i].sprite_index == old_index)
		{
			g->map.objects[i].sprite_index = new_index;
			return ;
		}
		i++;
	}
	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active
			&& g->map.enemies[i].sprite_index == old_index)
		{
			g->map.enemies[i].sprite_index = new_index;
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
		if (g->map.items[i].active
			&& in_pickup_range(g->player.pos, g->map.items[i].pos))
			collect_item(&g->map.items[i], g);
		i++;
	}
}

static bool	inside_collision_radius(t_position a, t_position b, double radius)
{
	double	dx;
	double	dy;

	dx = a.x - b.x;
	dy = a.y - b.y;
	return (dx * dx + dy * dy < radius * radius);
}

bool	is_object_blocking(t_position pos, t_game *g)
{
	double	radius;
	int		i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active && g->map.items[i].blocks_passage
			&& inside_collision_radius(pos, g->map.items[i].pos,
				ITEM_PICKUP_RADIUS + COLLISION_SAFETY))
			return (true);
		i++;
	}
	i = 0;
	while (i < g->map.object_count)
	{
		radius = g->map.objects[i].collision_radius + COLLISION_SAFETY;
		if (g->map.objects[i].blocks_passage
			&& inside_collision_radius(pos, g->map.objects[i].pos, radius))
			return (true);
		i++;
	}
	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active && inside_collision_radius(pos,
				g->map.enemies[i].pos,
				ENEMY_COLLISION_RADIUS + COLLISION_SAFETY))
			return (true);
		i++;
	}
	return (false);
}
