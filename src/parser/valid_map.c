/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:53:10 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:53:13 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	ft_setup_player(int *player, char **map, int lines, int width)
{
	int		vert;
	int		hor;
	char	pos;

	player[0] = -1;
	player[1] = -1;
	player[2] = 0;
	vert = 0;
	while (vert < lines)
	{
		hor = 0;
		while (hor < width)
		{
			pos = map[vert][hor];
			if ('N' == pos || 'S' == pos || 'W' == pos || 'E' == pos)
			{
				player[0] = vert;
				player[1] = hor;
				player[2] += 1;
			}
			hor++;
		}
		vert++;
	}
}

char	**ft_map_dup(char **map, int lines, int width)
{
	char	**copy;
	int		hor;
	int		vert;

	copy = malloc(sizeof(char *) * lines);
	if (!copy)
		return (ft_map_error(NULL, MAPMALLOC, 0, 0), NULL);
	vert = 0;
	while (vert < lines)
	{
		hor = -1;
		copy[vert] = malloc(sizeof(char) * (width + 1));
		if (!copy[vert])
			return (ft_map_error(copy, MAPLINEMALLOC, vert, 0), NULL);
		while (++hor < width)
			copy[vert][hor] = map[vert][hor];
		copy[vert][hor] = '\0';
		vert++;
	}
	return (copy);
}

int	ft_extract_map(char **map, char *mapfile, int *dims)
{
	int		fd;
	char	*line;

	fd = open(mapfile, O_RDONLY);
	if (!fd)
		return (ft_file_error(0, NULL, OPENF, 0));
	while (--dims[2])
	{
		line = get_next_line(fd, false);
		free(line);
	}
	line = get_next_line(fd, false);
	while (line != NULL)
	{
		ft_trimspaces(line);
		if (!ft_map_line(line))
			break ;
		map[dims[2]] = ft_calloc(dims[1] + 1, sizeof(char));
		if (!map[dims[2]])
			return (ft_file_error(fd, line, MAPLINEMALLOC, 0));
		ft_extract_line(line, map[dims[2]++], dims[1]);
		free(line);
		line = get_next_line(fd, false);
	}
	return (ft_file_error(fd, line, NULL, 1));
}

int	ft_valid_map_helper(char **map, char **cp, int lines, int *player)
{
	int	vals[3];
	int	corridor;

	corridor = 0;
	if (1 < player[2] || 0 == player[2])
		return (ft_free_map(cp, lines), ft_map_error(map, MAPPLAYER, lines, 0));
	vals[0] = lines;
	vals[1] = ft_strlen(map[0]);
	vals[2] = (int)map[player[0]][player[1]];
	ft_find_access_map(cp, player[0], player[1], vals);
	if (!ft_closed_map(cp, vals))
		return (ft_free_map(cp, lines), ft_map_error(map, MAPCLOSED, lines, 0));
	corridor = ft_hallway(map, vals);
	if (!corridor)
		return (ft_free_map(cp, lines), ft_map_error(map, HALWAY, lines, 0));
	return (ft_map_error(cp, NULL, lines, 1));
}

int	ft_valid_map(char **map, int lines, t_game *g)
{
	int				vals[2];
	char			**copy;
	int				player[3];
	const double	calibrate = COLLISION_SAFETY * 4;

	vals[0] = -1;
	vals[1] = 0;
	while (++vals[0] < lines)
	{
		if (!ft_invalid_line(map[vals[0]]))
			vals[1]++;
	}
	if (vals[1])
		return (ft_map_error(map, MAPCHAR, lines, 0));
	copy = ft_map_dup(map, lines, ft_strlen(map[0]));
	if (!copy)
		return (ft_map_error(map, NULL, lines, 0));
	ft_setup_player(player, map, lines, ft_strlen(map[0]));
	vals[0] = ft_valid_map_helper(map, copy, lines, player);
	if (!vals[0])
		return (ft_map_error(NULL, NULL, 0, 0));
	g->player.pos.y = player[0] + calibrate;
	g->player.pos.x = player[1] + calibrate;
	g->player.look = map[player[0]][player[1]];
	return (1);
}
