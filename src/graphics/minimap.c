/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	draw_minimap_items(t_game *g, t_coord origin)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active)
		{
			pos.x = origin.x + (int)(g->map.items[i].pos.x * MAP_SCALE) - 2;
			pos.y = origin.y + (int)(g->map.items[i].pos.y * MAP_SCALE) - 2;
			minimap_rect(g, pos, (t_coord){4, 4}, GREEN);
		}
		i++;
	}
}

static void	draw_minimap_exits(t_game *g, t_coord origin)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (i < g->map.exit_count)
	{
		pos.x = origin.x + g->map.exit_zones[i].x * MAP_SCALE + 2;
		pos.y = origin.y + g->map.exit_zones[i].y * MAP_SCALE + 2;
		minimap_rect(g, pos, (t_coord){MAP_SCALE - 4, MAP_SCALE - 4}, GREEN);
		i++;
	}
}

void	draw_minimap(t_game *g)
{
	t_coord	origin;
	t_coord	size;

	origin = (t_coord){PADDING, PADDING};
	size.x = g->map.width * MAP_SCALE + 8;
	size.y = g->map.height * MAP_SCALE + 8;
	minimap_rect(g, origin, size, HUD_BG);
	draw_map_cells(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_map_segments(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_exits(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_sprites(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_items(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_markers(g, (t_coord){origin.x + 4, origin.y + 4});
}
