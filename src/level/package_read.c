/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	extract_asset(int fd, char *header, t_dnk *dnk, t_game *g)
{
	t_dnk_asset	*asset;
	int			out;
	char		extra;

	if (dnk->asset_count >= DNK_MAX_ASSETS)
		return (false);
	asset = &dnk->assets[dnk->asset_count];
	if (sscanf(header, "ASSET %31s %7s %c", asset->key, asset->ext,
			&extra) != 2
		|| !valid_asset_part(asset->key) || !valid_asset_part(asset->ext))
		return (false);
	asset_path(dnk, asset, g);
	out = open(asset->path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (out < 0)
		return (false);
	if (!extract_asset_lines(fd, out))
		return (close(out), false);
	close(out);
	dnk->asset_count++;
	return (true);
}

char	*dup_dnk_line(char *line, t_game *g)
{
	char	*copy;

	copy = ft_strdup(line);
	if (!copy)
		error("Unable to allocate packed level line", g);
	return (s_alloc(copy, g));
}

bool	store_section_line(t_dnk *dnk, char *line, int state, t_game *g)
{
	if (state == 1)
	{
		if (dnk->cub_count >= DNK_MAX_LINES)
			return (false);
		dnk->cub_lines[dnk->cub_count++] = dup_dnk_line(line, g);
	}
	else if (state == 2)
	{
		if (dnk->sector_count >= DNK_MAX_LINES)
			return (false);
		dnk->sector_lines[dnk->sector_count++] = dup_dnk_line(line, g);
	}
	return (true);
}

bool	read_dnk(char *path, t_dnk *dnk, t_game *g)
{
	t_dnk_reader	reader;
	int				fd;
	char			*line;
	bool			ok;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	reader = (t_dnk_reader){dnk, g, 0, false};
	ok = true;
	line = get_next_line(fd, false);
	while (line && ok)
	{
		trim_eol(line);
		ok = read_dnk_line(fd, line, &reader);
		free(line);
		line = get_next_line(fd, false);
	}
	close(fd);
	free(line);
	return (ok && reader.magic_seen && reader.state == 0 && dnk->cub_count > 0);
}

bool	packed_cub_is_self_contained(t_dnk *dnk)
{
	char	key[32];
	char	value[LINE_SIZE];
	int		i;

	i = 0;
	while (i < dnk->cub_count)
	{
		if (sscanf(dnk->cub_lines[i], "%31s %299s", key, value) == 2
			&& ft_xpm_extension(value) && value[0] != '@')
			return (false);
		i++;
	}
	return (true);
}
