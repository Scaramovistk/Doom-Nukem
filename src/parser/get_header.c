/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:11 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:52:17 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	**ft_values_setup(int *readval)
{
	readval[0] = 0;
	readval[1] = 0;
	readval[2] = 0;
	readval[3] = 0;
	readval[4] = 0;
	readval[5] = 0;
	readval[6] = 0;
	readval[7] = 0;
	readval[8] = 0;
	return (NULL);
}

void	ft_setup_header(t_header *header)
{
	header->floor[0] = -1;
	header->floor[1] = -1;
	header->floor[2] = -1;
	header->ceiling[0] = -1;
	header->ceiling[1] = -1;
	header->ceiling[2] = -1;
	ft_bzero(header->no, LINE_SIZE);
	ft_bzero(header->ea, LINE_SIZE);
	ft_bzero(header->so, LINE_SIZE);
	ft_bzero(header->we, LINE_SIZE);
	ft_bzero(header->door, LINE_SIZE);
}

void	ft_get_xpm(char *dest, char *src, int *found, int *vals)
{
	if (LINE_SIZE - 1 <= ft_strlen(src) + vals[6] + wall)
		return ((void)ft_parsing_error(SXPM, 0));
	while (ft_isspace((unsigned char)*src))
		src++;
	ft_strcpy(dest, src);
	ft_trimspaces(dest);
	*found = true;
}

void	ft_get_rgb(int *rgb, int *found, char *line, int *values)
{
	char	**rgbs;
	char	*trim;

	rgbs = ft_split(line, ',');
	if (!rgbs)
		return ;
	if (ft_amount_of_commas(rgbs, values, line))
		return ;
	while (rgbs[values[0]] != NULL)
	{
		values[1] = -1;
		trim = ft_strtrim(rgbs[values[0]], " \t");
		values[2] = ft_file_error(0, rgbs[values[0]], NULL, 0);
		while (++values[1], trim[values[1]])
		{
			if (!ft_isdigit(trim[values[1]]))
				values[2] = true;
		}
		if (!values[2])
			rgb[values[0]] = ft_atoi(trim);
		values[0] = ft_file_error(0, trim, NULL, values[0] + 1);
	}
	free(rgbs);
	*found = 1;
}

int	ft_header_extractor(char *line, int *vals, t_header *header)
{
	int		values[9];
	char	*text;

	ft_values_setup(values);
	text = line + vals[6];
	if (ft_strncmp(text, "NO", wall) == 0)
		return (ft_get_xpm(header->no, text + wall, &vals[0], vals), 1);
	else if (ft_strncmp(text, "EA", wall) == 0)
		return (ft_get_xpm(header->ea, text + wall, &vals[1], vals), 1);
	else if (ft_strncmp(text, "SO", wall) == 0)
		return (ft_get_xpm(header->so, text + wall, &vals[2], vals), 1);
	else if (ft_strncmp(text, "WE", wall) == 0)
		return (ft_get_xpm(header->we, text + wall, &vals[3], vals), 1);
	else if (ft_strncmp(text, "F", color) == 0)
		return (ft_get_rgb(header->floor, &vals[4], text + color, values), 1);
	else if (ft_strncmp(text, "C", color) == 0)
		return (ft_get_rgb(header->ceiling, &vals[5], text + color, values), 1);
	else if (ft_strncmp(text, "DO", wall) == 0)
		return (ft_get_xpm(header->door, text + wall, &vals[7], vals), 1);
	else
		return (0);
}
