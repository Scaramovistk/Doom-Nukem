/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_grid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	populate_map_grid(char **map, int lines, int width, t_game *g)
{
	int	x;
	int	y;

	g->map.grid = calloc_s(lines, sizeof(t_block *), g);
	y = 0;
	while (y < lines)
	{
		x = 0;
		g->map.grid[y] = calloc_s(width, sizeof(t_block), g);
		while (x < width)
		{
			g->map.grid[y][x] = ft_convert_tblock(map[y][x]);
			if (g->map.grid[y][x] == SPRITE)
				g->map.grid[y][x] = EMPTY;
			x++;
		}
		y++;
	}
}
