/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_asset_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	get_sprite_frame(char *text, t_header *header, int *vals)
{
	int	found;
	int	frame;

	if (text[0] != 'S' || text[1] != 'P' || text[2] < '0' || text[2] > '7')
		return (0);
	found = 0;
	frame = text[2] - '0';
	ft_get_xpm(header->sprite_frame_textures[frame], text + 3, &found, vals);
	return (1);
}

static int	get_enemy_texture(char *text, t_header *header, int *vals)
{
	int	found;
	int	type;

	if (text[0] != 'E' || text[1] < '0' || text[1] > '4')
		return (0);
	found = 0;
	type = text[1] - '0';
	ft_get_xpm(header->enemy_texture[type], text + 2, &found, vals);
	return (1);
}

static int	get_decoration_texture(char *text, t_header *header, int *vals)
{
	int	found;
	int	type;

	if (text[0] != 'D' || text[1] < '1' || text[1] > '6')
		return (0);
	found = 0;
	type = text[1] - '1';
	ft_get_xpm(header->decoration_texture[type], text + 2, &found, vals);
	return (1);
}

int	ft_get_asset_texture(char *text, t_header *header, int *vals)
{
	if (get_sprite_frame(text, header, vals))
		return (1);
	if (get_enemy_texture(text, header, vals))
		return (1);
	return (get_decoration_texture(text, header, vals));
}
