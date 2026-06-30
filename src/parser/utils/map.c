/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:51:03 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:51:05 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

int	ft_map_line(const char *str)
{
	while (*str)
	{
		if (!ft_isprint(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_invalid_line(const char *str)
{
	const char	*valid_chars = "0123 NWSE";

	while (*str)
	{
		if (!ft_strchr(valid_chars, *str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_find_access_map(char **map, int vert, int hor, int *vals)
{
	char	pos;

	if (hor < 0 || vert >= vals[0] || vert < 0 || hor >= vals[1])
		return (0);
	pos = map[vert][hor];
	if (pos == '0' || pos == (char)vals[2] || pos == '2' || pos == '3')
	{
		map[vert][hor] = 'R';
		ft_find_access_map(map, vert + 1, hor, vals);
		ft_find_access_map(map, vert - 1, hor, vals);
		ft_find_access_map(map, vert, hor + 1, vals);
		ft_find_access_map(map, vert, hor - 1, vals);
	}
	return (1);
}

int	ft_is_escape(char **map, int vert, int hor, int *vals)
{
	if (vert == 0 || vert == vals[0] - 1 || hor == 0 || hor == vals[1] - 1)
		return (1);
	if (map[vert - 1][hor] == ' ' || map[vert + 1][hor] == ' ')
		return (1);
	if (map[vert][hor - 1] == ' ' || map[vert][hor + 1] == ' ')
		return (1);
	return (0);
}

int	ft_closed_map(char **map, int *vals)
{
	int	vert;
	int	hor;

	vert = 0;
	while (vert < vals[0])
	{
		hor = 0;
		while (hor < vals[1])
		{
			if (map[vert][hor] == 'R')
			{
				if (ft_is_escape(map, vert, hor, vals))
					return (0);
			}
			hor++;
		}
		vert++;
	}
	return (1);
}
