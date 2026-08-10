/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_cub_assets.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	write_cub_line(FILE *out, char *line)
{
	char	key[32];
	char	path[LINE_SIZE];
	size_t	len;

	if (header_asset_key(line, key, path))
		fprintf(out, "%s @%s\n", key, key);
	else
	{
		fprintf(out, "%s", line);
		len = ft_strlen(line);
		if (!len || line[len - 1] != '\n')
			fputc('\n', out);
	}
}

bool	write_sound_assets(FILE *out)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(SOUND_DIR);
	if (!dir)
		return (false);
	entry = readdir(dir);
	while (entry)
	{
		if (!write_sound_entry(out, entry->d_name))
			return (closedir(dir), false);
		entry = readdir(dir);
	}
	closedir(dir);
	return (true);
}

bool	write_hud_assets(FILE *out)
{
	const char *const	keys[HUD_ASSET_NB] = {"hud_pistol_idle",
		"hud_pistol_fire", "hud_blaster_idle", "hud_blaster_fire", "hud_ammo",
		"hud_item0", "hud_item1", "hud_item2", "hud_item3"};
	const char *const	paths[HUD_ASSET_NB] = {HUD_PISTOL_IDLE, HUD_PISTOL_FIRE,
		HUD_BLASTER_IDLE, HUD_BLASTER_FIRE, HUD_AMMO_ICON, HUD_ITEM0_ICON,
		HUD_ITEM1_ICON, HUD_ITEM2_ICON, HUD_ITEM3_ICON};
	int					i;

	i = 0;
	while (i < HUD_ASSET_NB)
	{
		if (!write_asset_block(out, keys[i], paths[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	write_cub_assets(FILE *out, char *src)
{
	int		fd;
	char	*line;
	char	key[32];
	char	path[LINE_SIZE];

	fd = open(src, O_RDONLY);
	if (fd < 0)
		return (false);
	line = get_next_line(fd, false);
	while (line)
	{
		if (header_asset_key(line, key, path) && !write_asset_block(out, key,
				path))
			return (close(fd), free(line), false);
		free(line);
		line = get_next_line(fd, false);
	}
	close(fd);
	return (true);
}

bool	is_map_source_line(char *line)
{
	int	i;
	int	has_map_char;

	i = 0;
	has_map_char = 0;
	while (line[i] && line[i] != '\n' && line[i] != '\r')
	{
		if (!ft_strchr("0123456789 NWSETHMXKLPIDCVBGQJabcdefghijklv", line[i]))
			return (false);
		if (line[i] != ' ')
			has_map_char = 1;
		i++;
	}
	return (has_map_char);
}
