/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

static int	ft_is_accessible(char pos, int player)
{
	return (pos == '0' || pos == (char)player || pos == '2' || pos == '3'
		|| pos == 'K' || (pos >= '6' && pos <= '9') || pos == 'H'
		|| pos == 'M' || pos == 'X' || pos == 'L' || pos == 'P' || pos == 'B'
		|| pos == 'I' || pos == 'D' || pos == 'C' || pos == 'V' || pos == 'v'
		|| pos == 'G' || pos == 'Q' || pos == 'J'
		|| (pos >= 'a' && pos <= 'f'));
}

static void	ft_add_neighbors(char **map, t_coord *queue, int *end,
		t_coord current)
{
	int	vals[3];
	int	i;
	int	x;
	int	y;

	vals[0] = queue[0].x;
	vals[1] = queue[0].y;
	vals[2] = queue[1].x;
	i = 0;
	while (i < 4)
	{
		x = current.x + (i == 0) - (i == 1);
		y = current.y + (i == 2) - (i == 3);
		if (x >= 0 && y >= 0 && x < vals[1] && y < vals[0]
			&& ft_is_accessible(map[y][x], vals[2]))
		{
			map[y][x] = 'R';
			queue[(*end)++] = (t_coord){x, y};
		}
		i++;
	}
}

int	ft_find_access_map(char **map, int vert, int hor, int *vals)
{
	t_coord	*queue;
	int		start;
	int		end;

	if (hor < 0 || vert >= vals[0] || vert < 0 || hor >= vals[1])
		return (0);
	queue = malloc(sizeof(*queue) * (vals[0] * vals[1] + 2));
	if (!queue)
		return (0);
	queue[0] = (t_coord){vals[0], vals[1]};
	queue[1] = (t_coord){vals[2], 0};
	start = 2;
	end = 2;
	if (ft_is_accessible(map[vert][hor], vals[2]))
	{
		map[vert][hor] = 'R';
		queue[end++] = (t_coord){hor, vert};
	}
	while (start < end)
		ft_add_neighbors(map, queue, &end, queue[start++]);
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
			if (map[vert][hor] == 'R'
				&& ft_is_escape(map, vert, hor, vals))
				return (0);
			hor++;
		}
		vert++;
	}
	return (1);
}
