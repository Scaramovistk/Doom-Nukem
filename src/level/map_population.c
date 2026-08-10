/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_population.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	add_interactables(char **map, int lines, int width, t_game *g)
{
	add_switches(map, lines, width, g);
	add_hazards(map, lines, width, g);
	add_messages(map, lines, width, g);
	add_exits(map, lines, width, g);
	add_elevators(map, lines, width, g);
	add_secrets(map, lines, width, g);
	add_locked_doors(map, lines, width, g);
}

void	ft_populate_map(char **map, int *vals, t_game *g)
{
	int	lines;
	int	width;

	lines = vals[0];
	width = vals[1];
	add_sprites(map, lines - vals[2], width, g);
	add_interactables(map, lines - vals[2], width, g);
	populate_map_grid(map, lines - vals[2], width, g);
	g->map.height = lines - vals[2];
	g->map.width = width;
	init_default_sectors(g);
	ft_free_map(map, lines);
}

void	ft_populate_info(t_header *h, t_game *g)
{
	populate_wall_assets(h, g);
	populate_plane_assets(h, g);
	populate_special_assets(h, g);
	populate_sprite_frames(h, g);
	populate_entity_assets(h, g);
	if (h->next_level[0])
		ft_strlcpy(g->level.next_level, h->next_level, LINE_SIZE);
}
