/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickup_sprite.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	relink_items(t_game *g, int old, int new)
{
	int	i;

	i = -1;
	while (++i < g->map.item_count)
		if (g->map.items[i].sprite_index == old)
			g->map.items[i].sprite_index = new;
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == old)
		g->map.vending_machine.sprite_index = new;
	if (g->map.has_flag && g->map.flag_sprite_index == old)
		g->map.flag_sprite_index = new;
	i = -1;
	while (++i < g->map.laptop_count)
		if (g->map.laptops[i].sprite_index == old)
			g->map.laptops[i].sprite_index = new;
}

static void	relink_decorations(t_game *g, int old, int new)
{
	int	i;

	i = -1;
	while (++i < g->map.decoration_count)
		if (g->map.decorations[i].sprite_index == old)
			g->map.decorations[i].sprite_index = new;
}

static void	relink_objects(t_game *g, int old, int new)
{
	int	i;

	i = -1;
	while (++i < g->map.object_count)
		if (g->map.objects[i].sprite_index == old)
			g->map.objects[i].sprite_index = new;
}

static void	relink_enemies(t_game *g, int old, int new)
{
	int	i;

	i = -1;
	while (++i < g->map.enemy_count)
		if (g->map.enemies[i].active
			&& g->map.enemies[i].sprite_index == old)
			g->map.enemies[i].sprite_index = new;
}

void	remove_item_sprite(t_item *item, t_game *g)
{
	int	last;

	last = g->map.sprite_count - 1;
	if (item->sprite_index != last)
	{
		g->map.sprites[item->sprite_index] = g->map.sprites[last];
		relink_items(g, last, item->sprite_index);
		relink_decorations(g, last, item->sprite_index);
		relink_objects(g, last, item->sprite_index);
		relink_enemies(g, last, item->sprite_index);
	}
	g->map.sprite_count--;
	item->sprite_index = -1;
}
