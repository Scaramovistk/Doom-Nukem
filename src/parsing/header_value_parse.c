/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_value_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

static int	has_comma(char *line)
{
	while (*line)
	{
		if (*line == ',')
			return (1);
		line++;
	}
	return (0);
}

void	ft_get_surface(char *dest, int *rgb, int *found, char *line)
{
	int		values[9];
	char	*trim;

	ft_values_setup(values);
	trim = ft_strtrim(line, " \t\n");
	if (!trim)
		return ;
	if (has_comma(trim))
		ft_get_rgb(rgb, found, line, values);
	else
		ft_get_xpm(dest, line, found, values);
	free(trim);
}
