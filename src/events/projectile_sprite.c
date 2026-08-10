/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_sprite.c                                :+:      :+:    :+:   */
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
		if (g->map.items[i].active && g->map.items[i].sprite_index == old)
			g->map.items[i].sprite_index = new;
}

static void	relink_decorations(t_game *g, int old, int new)
{
	int	i;

	i = -1;
	while (++i < g->map.decoration_count)
		if (g->map.decorations[i].sprite_index == old)
			g->map.decorations[i].sprite_index = new;
}

static void	relink_special(t_game *g, int old, int new)
{
	int	i;

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

static void	relink_moved_sprite(t_game *g, int old, int new)
{
	relink_items(g, old, new);
	relink_decorations(g, old, new);
	relink_special(g, old, new);
}

void	remove_projectile_target(t_game *g, int index, int damage)
{
	int	last;

	if (index < 0 || index >= g->map.sprite_count
		|| projectile_target_protected(g, index))
		return ;
	last = g->map.sprite_count - 1;
	if (index != last)
	{
		g->map.sprites[index] = g->map.sprites[last];
		relink_moved_sprite(g, last, index);
	}
	g->map.sprite_count--;
	g->hud.score += damage;
}
