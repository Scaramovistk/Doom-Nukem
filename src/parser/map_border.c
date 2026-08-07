/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_border.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
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
		if ((int)ft_strlen(map[y]) != width || map[y][0] != '1'
			|| map[y][width - 1] != '1')
			return (0);
		y++;
	}
	return (1);
}
