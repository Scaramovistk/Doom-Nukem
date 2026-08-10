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
	const char	*valid_chars;

	valid_chars = "0123456789 NWSETHMXKLPIDCVBGQJabcdefghijklv";

	while (*str)
	{
		if (!ft_strchr(valid_chars, *str))
			return (0);
		str++;
	}
	return (1);
}

static int	ft_is_accessible(char pos, int player)
{
	return (pos == '0' || pos == (char)player || pos == '2' || pos == '3'
		|| pos == 'K' || (pos >= '6' && pos <= '9') || pos == 'H'
		|| pos == 'M' || pos == 'X' || pos == 'L' || pos == 'P' || pos == 'B'
		|| pos == 'I' || pos == 'D' || pos == 'C' || pos == 'V' || pos == 'v'
		|| pos == 'G' || pos == 'Q' || pos == 'J'
		|| (pos >= 'a' && pos <= 'f'));
}

static void	ft_add_accessible_neighbors(char **map, t_coord *queue,
		int *queue_end, t_coord current, int *vals)
{
	const int	dx[4] = {1, -1, 0, 0};
	const int	dy[4] = {0, 0, 1, -1};
	int			i;
	int			x;
	int			y;

	i = 0;
	while (i < 4)
	{
		x = current.x + dx[i];
		y = current.y + dy[i++];
		if (x >= 0 && y >= 0 && x < vals[1] && y < vals[0]
			&& ft_is_accessible(map[y][x], vals[2]))
		{
			map[y][x] = 'R';
			queue[(*queue_end)++] = (t_coord){x, y};
		}
	}
}

int	ft_find_access_map(char **map, int vert, int hor, int *vals)
{
	t_coord	*queue;
	int		queue_start;
	int		queue_end;

	if (hor < 0 || vert >= vals[0] || vert < 0 || hor >= vals[1])
		return (0);
	queue = malloc(sizeof(*queue) * vals[0] * vals[1]);
	if (!queue)
		return (0);
	queue_start = 0;
	queue_end = 0;
	if (ft_is_accessible(map[vert][hor], vals[2]))
	{
		map[vert][hor] = 'R';
		queue[queue_end++] = (t_coord){hor, vert};
	}
	while (queue_start < queue_end)
		ft_add_accessible_neighbors(map, queue, &queue_end,
			queue[queue_start++], vals);
	free(queue);
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
