/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_token_count.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	count_sprites(char **map, int lines, int width)
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
			if (ft_is_enemy_char(map[vert][hor]))
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

int	count_flags(char **map, int lines, int width)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
			count += (map[y][x++] == 'G');
		y++;
	}
	return (count);
}

bool	ft_is_item_char(char c)
{
	return (c >= '6' && c <= '9');
}

int	count_items(char **map, int lines, int width)
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
			if (ft_is_item_char(map[vert][hor]))
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

int	item_default_quantity(int type)
{
	const int	amounts[ITEM_TYPES_NB] = {25, 10, 1, 5};

	if (type < 0 || type >= ITEM_TYPES_NB)
		return (1);
	return (amounts[type]);
}
