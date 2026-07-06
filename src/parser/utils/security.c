/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:51:34 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:51:44 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

static int	ft_is_wall_like(char c)
{
	return (c == '1' || c == '4' || c == '5' || c == 'T');
}

int	ft_is_halway(char **map, int vert, int hor)
{
	int	up;
	int	down;
	int	left;
	int	right;

	left = map[vert][hor - 1];
	right = map[vert][hor + 1];
	up = map[vert - 1][hor];
	down = map[vert + 1][hor];
	if (ft_is_wall_like(left) && ft_is_wall_like(right)
		&& up == '0' && down == '0')
		return (1);
	if (ft_is_wall_like(up) && ft_is_wall_like(down)
		&& left == '0' && right == '0')
		return (1);
	return (0);
}

int	ft_hallway(char **map, int *vals)
{
	int	vert;
	int	hor;
	int	corridor;

	vert = -1;
	while (++vert < vals[0])
	{
		hor = -1;
		while (++hor < vals[1])
		{
			corridor = 0;
			if (map[vert][hor] == '2')
			{
				corridor = ft_is_halway(map, vert, hor);
				if (!corridor)
					return (0);
			}
		}
	}
	return (1);
}
