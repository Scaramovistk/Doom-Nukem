/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_markers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	draw_flag_base(t_game *g, t_coord origin)
{
	t_coord	pos;

	if (!g->map.has_flag)
		return ;
	pos.x = origin.x + (int)(g->map.flag_base.x * MAP_SCALE) - 3;
	pos.y = origin.y + (int)(g->map.flag_base.y * MAP_SCALE) - 3;
	minimap_rect(g, pos, (t_coord){6, 6}, BLUE);
}

static void	draw_player_marker(t_game *g, t_coord origin)
{
	t_coord	pos;
	t_coord	dir;

	pos.x = origin.x + (int)(g->player.pos.x * MAP_SCALE);
	pos.y = origin.y + (int)(g->player.pos.y * MAP_SCALE);
	minimap_rect(g, (t_coord){pos.x - 3, pos.y - 3},
		(t_coord){6, 6}, WHITE);
	dir.x = pos.x + (int)(cos(g->player.orientation) * 12);
	dir.y = pos.y + (int)(sin(g->player.orientation) * 12);
	minimap_rect(g, (t_coord){dir.x - 1, dir.y - 1},
		(t_coord){3, 3}, WHITE);
}

void	draw_minimap_markers(t_game *g, t_coord origin)
{
	draw_flag_base(g, origin);
	draw_player_marker(g, origin);
}
