/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:37 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:52:40 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	ft_is_file(char *path)
{
	int	fd;

	fd = open(path, DIRREAD);
	if (fd < 0)
		return (1);
	close(fd);
	return (0);
}

int	ft_cub_extension(char *path)
{
	int				len;
	const char		*cub = ".cub";
	const size_t	size = ft_strlen(cub);

	len = ft_strlen(path);
	if (size <= (size_t)len)
	{
		path += len - 4;
		if (ft_strcmp(path, cub) == 0)
			return (1);
	}
	return (0);
}

int	ft_parse_arg(char *map)
{
	int	fd;

	fd = open(map, O_RDONLY);
	if (fd == -1)
		return (ft_parsing_error(NOFILE, 0));
	close(fd);
	if (!ft_is_file(map))
		return (ft_parsing_error(DIRFILE, 0));
	if (!ft_cub_extension(map))
		return (ft_parsing_error(CUBFILE, 0));
	return (1);
}

int	ft_parse_file(int argc, char *argv[], t_game *g)
{
	int			values[9];
	t_header	header;

	if (argc != 2)
		return (ft_parsing_error(ARGSP, 0));
	ft_setup_header(&header);
	ft_values_setup(values);
	values[6] = 1;
	if (ft_parse_arg(argv[1]))
	{
		values[1] = ft_get_header(argv[1], &values[6], &header);
		if (!values[6])
			return (ft_parsing_error(HEADERP, 0));
		values[2] = ft_skip_blanks(argv[1], values[1]);
		values[3] = values[1];
		values[1] += values[2];
		values[5] = ft_get_map(argv[1], values, &values[0], g);
		if (!values[5])
			return (ft_parsing_error(MAPP, 0));
		return (ft_populate_info(&header, g), 1);
	}
	return (0);
}

int	ft_amount_of_commas(char **rgbs, int *values, char *line)
{
	while (line[values[4]])
	{
		if (line[values[4]] == ',')
			values[5]++;
		values[4]++;
	}
	while (rgbs[values[3]] != NULL)
		values[3]++;
	if (3 < values[3] || values[5] != 2)
	{
		while (rgbs[values[0]] != NULL)
		{
			free(rgbs[values[0]]);
			values[0]++;
		}
		free(rgbs);
		return (ft_parsing_error(RGBF, 1));
	}
	return (0);
}
