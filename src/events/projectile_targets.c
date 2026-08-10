/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_targets.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_item_sprite(t_game *g, int index)
{
	int	i;

	i = -1;
	while (++i < g->map.item_count)
		if (g->map.items[i].active && g->map.items[i].sprite_index == index)
			return (true);
	return (false);
}

bool	projectile_is_decoration(t_game *g, int index)
{
	int	i;

	i = -1;
	while (++i < g->map.decoration_count)
		if (g->map.decorations[i].sprite_index == index)
			return (true);
	return (false);
}

static bool	is_vending_sprite(t_game *g, int index)
{
	return (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == index);
}

static bool	is_laptop_sprite(t_game *g, int index)
{
	int	i;

	i = -1;
	while (++i < g->map.laptop_count)
		if (g->map.laptops[i].sprite_index == index)
			return (true);
	return (false);
}

bool	projectile_target_protected(t_game *g, int index)
{
	return (is_item_sprite(g, index) || is_vending_sprite(g, index)
		|| is_laptop_sprite(g, index));
}
