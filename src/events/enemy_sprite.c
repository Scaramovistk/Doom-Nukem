/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_sprite.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	replace_item_sprite(t_game *g, int last, int next)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].sprite_index == last)
			g->map.items[i].sprite_index = next;
		i++;
	}
}

static void	replace_enemy_sprite(t_game *g, int last, int next)
{
	int	i;

	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active
			&& g->map.enemies[i].sprite_index == last)
			g->map.enemies[i].sprite_index = next;
		i++;
	}
}

static void	replace_decoration_sprite(t_game *g, int last, int next)
{
	int	i;

	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == last)
			g->map.decorations[i].sprite_index = next;
		i++;
	}
}

static void	replace_object_sprite(t_game *g, int last, int next)
{
	int	i;

	i = 0;
	while (i < g->map.object_count)
	{
		if (g->map.objects[i].sprite_index == last)
			g->map.objects[i].sprite_index = next;
		i++;
	}
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == last)
		g->map.vending_machine.sprite_index = next;
	if (g->map.has_flag && g->map.flag_sprite_index == last)
		g->map.flag_sprite_index = next;
	i = -1;
	while (++i < g->map.laptop_count)
		if (g->map.laptops[i].sprite_index == last)
			g->map.laptops[i].sprite_index = next;
}

void	remove_enemy_sprite(t_game *g, t_enemy *enemy)
{
	int	last;

	last = g->map.sprite_count - 1;
	if (enemy->sprite_index < 0 || enemy->sprite_index > last)
		return ;
	if (enemy->sprite_index != last)
	{
		g->map.sprites[enemy->sprite_index] = g->map.sprites[last];
		replace_item_sprite(g, last, enemy->sprite_index);
		replace_enemy_sprite(g, last, enemy->sprite_index);
		replace_decoration_sprite(g, last, enemy->sprite_index);
		replace_object_sprite(g, last, enemy->sprite_index);
	}
	g->map.sprite_count--;
	enemy->sprite_index = -1;
}
