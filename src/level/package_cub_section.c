/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_cub_section.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	write_sound_entry(FILE *out, char *filename)
{
	char	path[LINE_SIZE];
	char	key[32];
	char	name[32];
	char	*extension;

	extension = ft_strrchr(filename, '.');
	if (!extension || (ft_strcmp(extension, ".wav")
			&& ft_strcmp(extension, ".ogg") && ft_strcmp(extension, ".mp3")))
		return (true);
	ft_strlcpy(key, "sound_", sizeof(key));
	ft_strlcpy(name, filename, sizeof(name));
	extension = ft_strrchr(name, '.');
	if (extension)
		*extension = '\0';
	ft_strlcat(key, name, sizeof(key));
	snprintf(path, LINE_SIZE, "%s%s", SOUND_DIR, filename);
	return (write_asset_block(out, key, path));
}

int	count_map_lines(char **lines, int count)
{
	int	i;
	int	map_count;

	i = 0;
	map_count = 0;
	while (i < count)
	{
		if (is_map_source_line(lines[i]))
			map_count++;
		i++;
	}
	return (map_count);
}

void	write_sector_rows(FILE *out, char **lines, int count, int width)
{
	int	i;
	int	x;
	int	map_y;
	int	map_count;

	i = 0;
	map_y = 0;
	map_count = count_map_lines(lines, count);
	while (i < count)
	{
		if (!is_map_source_line(lines[i++]))
			continue ;
		x = 0;
		while (x++ < width)
		{
			if (map_y > map_count / 2)
				fputc('1', out);
			else
				fputc('0', out);
		}
		fputc('\n', out);
		map_y++;
	}
}

void	free_cub_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
}

void	write_cub_section(FILE *out, char **lines, int count)
{
	int	i;

	fprintf(out, "BEGIN_CUB\n");
	i = 0;
	while (i < count)
		write_cub_line(out, lines[i++]);
	fprintf(out, "END_CUB\n");
}
