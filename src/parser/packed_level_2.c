/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_level_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	write_hex_line(int fd, const char *line)
{
	unsigned char	byte;
	int				high;
	int				low;

	while (*line)
	{
		if (*line == ' ' || *line == '\t')
		{
			line++;
			continue ;
		}
		high = hex_value(*line++);
		if (!*line)
			return (false);
		low = hex_value(*line++);
		if (high < 0 || low < 0)
			return (false);
		byte = (unsigned char)((high << 4) | low);
		if (write(fd, &byte, 1) != 1)
			return (false);
	}
	return (true);
}

bool	valid_asset_part(const char *text)
{
	if (!*text)
		return (false);
	while (*text)
	{
		if (!ft_isalnum(*text) && *text != '_')
			return (false);
		text++;
	}
	return (true);
}

bool	is_sound_asset(const char *key)
{
	return (starts_with(key, "sound_"));
}

bool	is_hud_asset(const char *key)
{
	return (starts_with(key, "hud_"));
}

void	asset_path(t_dnk *dnk, t_dnk_asset *asset, t_game *g)
{
	ft_strlcpy(asset->path, dnk->dir, LINE_SIZE);
	if (is_sound_asset(asset->key))
	{
		append_path(asset->path, "/sounds/");
		append_path(asset->path, asset->key + 6);
		ft_strlcpy(g->audio.sound_dir, dnk->dir, LINE_SIZE);
		append_path(g->audio.sound_dir, "/sounds/");
	}
	else if (is_hud_asset(asset->key))
	{
		append_path(asset->path, "/hud/");
		append_path(asset->path, asset->key + 4);
	}
	else
	{
		append_path(asset->path, "/");
		append_path(asset->path, asset->key);
	}
	append_path(asset->path, ".");
	append_path(asset->path, asset->ext);
}
