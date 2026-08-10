/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interaction_population.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	count_char(char **map, int lines, int width, char target)
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

void	add_switches(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.switch_count = count_char(map, lines, width, 'T');
	if (!g->map.switch_count)
		return ;
	g->map.switches = calloc_s(g->map.switch_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'T')
				g->map.switches[i++] = (t_coord){hor, vert};
	}
}

void	add_hazards(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.hazard_count = count_char(map, lines, width, 'H');
	if (!g->map.hazard_count)
		return ;
	g->map.hazard_zones = calloc_s(g->map.hazard_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'H')
				g->map.hazard_zones[i++] = (t_coord){hor, vert};
	}
}

void	add_messages(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.message_count = count_char(map, lines, width, 'M');
	if (!g->map.message_count)
		return ;
	g->map.message_zones = calloc_s(g->map.message_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'M')
				g->map.message_zones[i++] = (t_coord){hor, vert};
	}
}

void	add_exits(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.exit_count = count_char(map, lines, width, 'X');
	if (!g->map.exit_count)
		return ;
	g->map.exit_zones = calloc_s(g->map.exit_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'X')
				g->map.exit_zones[i++] = (t_coord){hor, vert};
	}
}
