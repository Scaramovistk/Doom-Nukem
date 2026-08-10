/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_entity_lookup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_texture	*item_sprite_texture(t_sprite_draw *s, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active
			&& g->map.items[i].sprite_index == s->sprite_index
			&& g->map.items[i].type >= 0
			&& g->map.items[i].type < ITEM_TYPES_NB)
			return (&g->assets.item_icons[g->map.items[i].type]);
		i++;
	}
	return (NULL);
}

static t_texture	*decoration_sprite_texture(t_sprite_draw *s, t_game *g)
{
	int	i;
	int	type;

	i = 0;
	while (i < g->map.decoration_count)
	{
		type = g->map.decorations[i].type;
		if (g->map.decorations[i].sprite_index == s->sprite_index
			&& g->assets.decoration_icons[type].img.ptr)
			return (&g->assets.decoration_icons[type]);
		i++;
	}
	return (NULL);
}

static t_texture	*enemy_sprite_texture(t_sprite_draw *s, t_game *g)
{
	int	i;
	int	type;

	i = 0;
	while (i < g->map.enemy_count)
	{
		type = g->map.enemies[i].type;
		if (g->map.enemies[i].active
			&& g->map.enemies[i].sprite_index == s->sprite_index && type >= 0
			&& type < ENEMY_TYPES_NB && g->assets.enemy_icons[type].img.ptr)
			return (&g->assets.enemy_icons[type]);
		i++;
	}
	return (NULL);
}

t_texture	*entity_sprite_texture(t_sprite_draw *s, t_game *g)
{
	t_texture	*texture;

	texture = item_sprite_texture(s, g);
	if (!texture)
		texture = decoration_sprite_texture(s, g);
	if (!texture)
		texture = enemy_sprite_texture(s, g);
	return (texture);
}
