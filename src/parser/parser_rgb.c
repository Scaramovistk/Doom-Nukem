/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rgb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
