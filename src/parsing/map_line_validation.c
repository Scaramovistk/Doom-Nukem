/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_line_validation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:51:03 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:51:05 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
