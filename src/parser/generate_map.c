/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:00 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:11:27 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_block	ft_convert_tblock(char c)
{
	if (c == '0')
		return (EMPTY);
	else if (c == '1')
		return (WALL);
	else if (c == 'S' || c == 'W' || c == 'N' || c == 'E')
		return (PLAYER);
	else if (c == '2' || c == 'P' || c == 'B')
		return (DOOR);
	else if (c == '3' || c == 'K' || c == 'I' || c == 'D' || c == 'C')
		return (SPRITE);
	else if (c == '4')
		return (TRANSPARENT_WALL);
	else if (c == '5' || c == 'T' || (c >= 'g' && c <= 'l'))
		return (DECAL_WALL);
	else if (c == 'L')
		return (WALL);
	else if ((c >= '6' && c <= '9') || c == 'H' || c == 'M' || c == 'X'
		|| c == 'V' || c == 'v' || c == 'Q' || c == 'J' || c == 'G' || (c >= 'a'
			&& c <= 'f'))
		return (EMPTY);
	else
		return (NULL_BLOCK);
}

bool	ft_is_enemy_char(char c)
{
	return (c == '3' || c == 'K' || c == 'I' || c == 'D' || c == 'C');
}

int	count_map_char(char **map, int lines, int width, char target)
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
			count += (map[y][x++] == target);
		y++;
	}
	return (count);
}

int	count_decorations(char **map, int lines, int width)
{
	int	type;
	int	count;

	count = count_map_char(map, lines, width, 'L');
	type = 0;
	while (type < DECORATION_TYPES_NB)
	{
		count += count_map_char(map, lines, width, 'a' + type);
		count += count_map_char(map, lines, width, 'g' + type++);
	}
	return (count);
}

int	count_billboard_decorations(char **map, int lines, int width)
{
	int	type;
	int	count;

	count = count_map_char(map, lines, width, 'L');
	type = 0;
	while (type < DECORATION_TYPES_NB)
		count += count_map_char(map, lines, width, 'a' + type++);
	return (count);
}
