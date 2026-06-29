/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:22 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:52:26 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	ft_get_height(char *mapfile, int skip, int correction, int restfile)
{
	int		fd;
	char	*line;

	fd = open(mapfile, O_RDONLY);
	if (!fd)
		return (ft_map_error(NULL, OPENF, 0, 0));
	while (--skip)
	{
		line = get_next_line(fd, false);
		free(line);
	}
	skip += correction;
	line = get_next_line(fd, false);
	while (line != NULL)
	{
		ft_trimspaces(line);
		if (restfile && !ft_map_line(line))
			break ;
		skip++;
		free(line);
		line = get_next_line(fd, false);
	}
	return (ft_file_error(fd, line, NULL, skip));
}

int	ft_get_width(char *mapfile, int skip, int *line_info)
{
	int		fd;
	char	*line;

	fd = open(mapfile, O_RDONLY);
	if (!fd)
		return (ft_map_error(NULL, OPENF, 0, 0));
	while (--skip)
	{
		line = get_next_line(fd, false);
		free(line);
	}
	line = get_next_line(fd, false);
	while (line != NULL)
	{
		ft_trimspaces(line);
		skip++;
		if (!ft_map_line(line))
			continue ;
		line_info[0] = ft_strlen(line);
		if (line_info[1] < line_info[0])
			line_info[1] = line_info[0];
		free(line);
		line = get_next_line(fd, false);
	}
	return (ft_file_error(fd, line, NULL, line_info[1]));
}

char	**ft_create_map(char *mapfile, int *skip, int height, int *width)
{
	char	**map;
	int		line_info[2];

	skip[5] = skip[4] - skip[2];
	line_info[0] = 0;
	line_info[1] = 0;
	skip[6] = ft_get_height(mapfile, skip[1], skip[2], 1);
	if (height != skip[6])
		return (ft_map_error(NULL, CORRUPTMAP, 0, 0), NULL);
	if (MAX_MAP_HEIGHT < skip[6])
		return (ft_map_error(NULL, EXCESSH, 0, 0), NULL);
	*width = ft_get_width(mapfile, skip[1], line_info);
	if (MAX_MAP_WIDTH < *width)
		return (ft_map_error(NULL, EXCESSW, 0, 0), NULL);
	map = ft_calloc(height + 1, sizeof(char *));
	if (!map)
		return (ft_map_error(NULL, MAPMALLOC, 0, 0), NULL);
	return (map);
}

void	ft_extract_line(char *src, char *dest, int chars)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
		chars--;
	}
	if (0 < chars)
	{
		while (chars)
		{
			dest[i] = ' ';
			i++;
			chars--;
		}
	}
	dest[i] = '\0';
}

int	ft_get_map(char *mapfile, int *skip, int *succes, t_game *g)
{
	int		lines;
	int		width;
	char	**map;
	int		dims[3];

	width = 0;
	lines = ft_get_height(mapfile, skip[1], skip[2], 0);
	if (!lines)
		return (ft_map_error(NULL, NOMAP, 0, 0));
	skip[4] = skip[3] + lines - 1;
	map = ft_create_map(mapfile, skip, lines, &width);
	if (!map)
		return (0);
	if (!width)
		return (ft_map_error(NULL, PROBLEMW, 0, 0));
	dims[0] = lines;
	dims[1] = width;
	dims[2] = skip[1];
	*succes = ft_extract_map(map, mapfile, dims);
	if (!(*succes))
		return (ft_map_error(map, PROBLEME, lines, 0));
	if (!ft_valid_map(map, dims[2], g))
		return (ft_map_error(NULL, VALIDMAP, 0, 0));
	dims[2] = skip[2];
	return (ft_populate_map(map, dims, g), 1);
}
