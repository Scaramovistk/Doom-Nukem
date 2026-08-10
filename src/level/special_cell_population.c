/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_cell_population.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	count_char_at(char **map, int lines, int width, char target)
{
	int	count;
	int	vert;
	int	hor;

	count = 0;
	vert = 0;
	while (vert < lines)
	{
		hor = 0;
		while (hor < width)
		{
			if (map[vert][hor] == target)
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

void	add_elevators(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.elevator_count = count_char_at(map, lines, width, 'L');
	if (!g->map.elevator_count)
		return ;
	g->map.elevators = calloc_s(g->map.elevator_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'L')
				g->map.elevators[i++] = (t_coord){hor, vert};
	}
}

void	add_secrets(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.secret_count = count_char_at(map, lines, width, 'P');
	if (!g->map.secret_count)
		return ;
	g->map.secrets = calloc_s(g->map.secret_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'P')
				g->map.secrets[i++] = (t_coord){hor, vert};
	}
}

void	add_locked_doors(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.locked_door_count = count_char_at(map, lines, width, 'B');
	if (!g->map.locked_door_count)
		return ;
	g->map.locked_doors = calloc_s(g->map.locked_door_count, sizeof(t_coord),
			g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'B')
				g->map.locked_doors[i++] = (t_coord){hor, vert};
	}
}
