/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:11 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	get_cardinal_texture(char *text, t_header *header, int *vals)
{
	if (ft_strncmp(text, "NO", wall) == 0)
		return (ft_get_xpm(header->no, text + wall, &vals[0], vals), 1);
	if (ft_strncmp(text, "EA", wall) == 0)
		return (ft_get_xpm(header->ea, text + wall, &vals[1], vals), 1);
	if (ft_strncmp(text, "SO", wall) == 0)
		return (ft_get_xpm(header->so, text + wall, &vals[2], vals), 1);
	if (ft_strncmp(text, "WE", wall) == 0)
		return (ft_get_xpm(header->we, text + wall, &vals[3], vals), 1);
	return (0);
}

static int	get_base_texture(char *text, t_header *header, int *vals)
{
	if (ft_strncmp(text, "F", color) == 0)
		return (ft_get_surface(header->floor_texture, header->floor,
				&vals[4], text + color), 1);
	if (ft_strncmp(text, "C", color) == 0)
		return (ft_get_surface(header->ceiling_texture, header->ceiling,
				&vals[5], text + color), 1);
	if (ft_strncmp(text, "DO", wall) == 0)
		return (ft_get_xpm(header->door, text + wall, &vals[7], vals), 1);
	return (0);
}

static int	get_optional_first(char *text, t_header *header, int *vals)
{
	int	found;

	found = 0;
	if (ft_strncmp(text, "SK", wall) == 0)
		return (ft_get_xpm(header->sky_texture, text + wall, &found, vals), 1);
	if (ft_strncmp(text, "SP", wall) == 0)
		return (ft_get_xpm(header->sprite_texture, text + wall,
				&found, vals), 1);
	if (ft_strncmp(text, "VM", wall) == 0)
		return (ft_get_xpm(header->vending_machine_texture, text + wall,
				&found, vals), 1);
	return (0);
}

static int	get_optional_last(char *text, t_header *header, int *vals)
{
	int	found;

	found = 0;
	if (ft_strncmp(text, "LT", wall) == 0)
		return (ft_get_xpm(header->laptop_texture, text + wall,
				&found, vals), 1);
	if (ft_strncmp(text, "TR", wall) == 0)
		return (ft_get_xpm(header->transparent_texture, text + wall,
				&found, vals), 1);
	if (ft_strncmp(text, "DC", wall) == 0)
		return (ft_get_xpm(header->decal_texture, text + wall,
				&found, vals), 1);
	if (ft_strncmp(text, "NEXT", 4) == 0)
		return (ft_get_xpm(header->next_level, text + 4, &found, vals), 1);
	return (0);
}

int	ft_header_extractor(char *line, int *vals, t_header *header)
{
	char	*text;

	text = line + vals[6];
	if (ft_get_asset_texture(text, header, vals))
		return (1);
	if (get_cardinal_texture(text, header, vals))
		return (1);
	if (get_base_texture(text, header, vals))
		return (1);
	if (get_optional_first(text, header, vals))
		return (1);
	return (get_optional_last(text, header, vals));
}
