/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_lookup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_decoration	*sprite_decoration(t_sprite_draw *s, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == s->sprite_index)
			return (&g->map.decorations[i]);
		i++;
	}
	return (NULL);
}

t_world_object	*sprite_object(t_sprite_draw *s, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.object_count)
	{
		if (g->map.objects[i].sprite_index == s->sprite_index)
			return (&g->map.objects[i]);
		i++;
	}
	return (NULL);
}

static t_texture	*special_sprite_texture(t_sprite_draw *s, t_game *g)
{
	int	i;

	if (g->map.has_flag && !g->map.flag_carried
		&& g->map.flag_sprite_index == s->sprite_index)
		return (&g->assets.item_icons[ITEM_ARTIFACT]);
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == s->sprite_index
		&& g->assets.vending_machine.img.ptr)
		return (&g->assets.vending_machine);
	i = 0;
	while (i < g->map.laptop_count)
	{
		if (g->map.laptops[i].sprite_index == s->sprite_index
			&& g->assets.laptop.img.ptr)
			return (&g->assets.laptop);
		i++;
	}
	return (NULL);
}

static t_texture	*directional_sprite(t_sprite_draw *s, t_game *g)
{
	double	angle;
	int		frame;

	if (!g->assets.has_sprite_frames)
		return (&g->assets.textures[SPRITE_T]);
	angle = normalize_angle(atan2(g->player.pos.y - s->pos.y,
				g->player.pos.x - s->pos.x));
	frame = (int)((angle + (M_PI / SPRITE_FRAME_NB)) / (2 * M_PI
				/ SPRITE_FRAME_NB)) % SPRITE_FRAME_NB;
	return (&g->assets.sprite_frames[frame]);
}

t_texture	*get_sprite_texture(t_sprite_draw *s, t_game *g)
{
	t_world_object	*object;
	t_texture		*texture;

	object = sprite_object(s, g);
	if (object && object->texture != SPRITE_T)
		return (&g->assets.textures[object->texture]);
	texture = special_sprite_texture(s, g);
	if (texture)
		return (texture);
	texture = entity_sprite_texture(s, g);
	if (texture)
		return (texture);
	return (directional_sprite(s, g));
}
