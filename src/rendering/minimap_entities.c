/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_entities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	sprite_hidden(t_game *g, int index)
{
	int	i;

	if (g->map.has_flag && g->map.flag_carried
		&& g->map.flag_sprite_index == index)
		return (true);
	i = -1;
	while (++i < g->map.laptop_count)
		if (g->map.laptops[i].sprite_index == index)
			return (true);
	return (false);
}

static int	sprite_color(t_game *g, int index)
{
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == index)
		return (GREEN);
	if (g->map.has_flag && g->map.flag_sprite_index == index)
		return (YELLOW);
	return (RED);
}

void	draw_minimap_sprites(t_game *g, t_coord origin)
{
	t_coord	pos;
	int		i;

	i = -1;
	while (++i < g->map.sprite_count)
	{
		if (sprite_hidden(g, i))
			continue ;
		pos.x = origin.x + (int)(g->map.sprites[i].x * MAP_SCALE) - 2;
		pos.y = origin.y + (int)(g->map.sprites[i].y * MAP_SCALE) - 2;
		minimap_rect(g, pos, (t_coord){4, 4}, sprite_color(g, i));
	}
}
