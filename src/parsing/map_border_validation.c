/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_border_validation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	solid_row(char *row, int width)
{
	int	x;

	x = 0;
	while (x < width)
	{
		if (row[x] != '1')
			return (0);
		x++;
	}
	return (1);
}

int	ft_solid_map_border(char **map, int lines, int width)
{
	int	y;

	if (!map || lines < 3 || width < 3)
		return (0);
	if (!solid_row(map[0], width) || !solid_row(map[lines - 1], width))
		return (0);
	y = 1;
	while (y < lines - 1)
	{
		if ((int)ft_strlen(map[y]) != width || map[y][0] != '1' || map[y][width
			- 1] != '1')
			return (0);
		y++;
	}
	return (1);
}

void	ft_set_player_from_map(t_game *g, char **map, int *player)
{
	const double	calibrate = COLLISION_SAFETY * 4;

	g->player.pos.y = player[0] + calibrate;
	g->player.pos.x = player[1] + calibrate;
	g->map.flag_base = (t_position){player[1] + 0.5, player[0] + 0.5};
	g->player.look = map[player[0]][player[1]];
}
