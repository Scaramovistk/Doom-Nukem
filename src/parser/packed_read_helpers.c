/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_read_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	extract_asset_lines(int fd, int out)
{
	char	*line;

	line = get_next_line(fd, false);
	while (line)
	{
		trim_eol(line);
		if (ft_strcmp(line, "END_ASSET") == 0)
			return (free(line), true);
		if (!write_hex_line(out, line))
			return (free(line), false);
		free(line);
		line = get_next_line(fd, false);
	}
	return (false);
}

static bool	set_reader_section(char *line, t_dnk_reader *reader)
{
	int	wanted;

	wanted = 0;
	if (!ft_strcmp(line, "BEGIN_CUB"))
		wanted = 1;
	else if (!ft_strcmp(line, "BEGIN_SECTORS"))
		wanted = 2;
	if (wanted)
	{
		if (reader->state != 0 || (wanted == 1 && reader->dnk->cub_count != 0))
			return (false);
		reader->state = wanted;
		return (true);
	}
	if (ft_strcmp(line, "END_CUB") && ft_strcmp(line, "END_SECTORS"))
		return (false);
	if ((!ft_strcmp(line, "END_CUB") && reader->state != 1)
		|| (!ft_strcmp(line, "END_SECTORS") && reader->state != 2))
		return (false);
	reader->state = 0;
	return (true);
}

bool	read_dnk_line(int fd, char *line, t_dnk_reader *reader)
{
	if (!reader->magic_seen)
	{
		reader->magic_seen = true;
		return (ft_strcmp(line, DNK_MAGIC) == 0);
	}
	if (starts_with(line, "BEGIN_") || starts_with(line, "END_"))
		return (set_reader_section(line, reader));
	if (starts_with(line, "ASSET "))
		return (reader->state == 0 && extract_asset(fd, line, reader->dnk,
				reader->g));
	return (reader->state != 0 && store_section_line(reader->dnk, line,
			reader->state, reader->g));
}
