/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_level_8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	max_map_width(char **lines, int count)
{
	int	i;
	int	width;
	int	len;

	i = 0;
	width = 0;
	while (i < count)
	{
		if (is_map_source_line(lines[i]))
		{
			len = ft_strlen(lines[i]);
			if (len && lines[i][len - 1] == '\n')
				len--;
			if (width < len)
				width = len;
		}
		i++;
	}
	return (width);
}

void	write_default_sector_grid(FILE *out, char **lines, int count)
{
	int	width;

	width = max_map_width(lines, count);
	fprintf(out, "BEGIN_SECTORS\n");
	fprintf(out, "SECTOR 0 0.00 1.00 0.00 0.00 255\n");
	fprintf(out, "SECTOR 1 0.20 1.20 0.00 0.00 190\n");
	fprintf(out, "GRID\n");
	write_sector_rows(out, lines, count, width);
	fprintf(out, "END_SECTORS\n");
}

bool	write_sector_sidecar(FILE *out, char *src)
{
	char	path[LINE_SIZE];
	char	line[LINE_SIZE];
	char	*extension;
	FILE	*sidecar;

	ft_strlcpy(path, src, LINE_SIZE);
	extension = ft_strrchr(path, '.');
	if (extension && ft_strcmp(extension, ".cub") == 0)
		ft_strlcpy(extension, ".sectors", LINE_SIZE - (extension - path));
	else
		ft_strlcat(path, ".sectors", LINE_SIZE);
	sidecar = fopen(path, "r");
	if (!sidecar)
		return (false);
	line[0] = '\0';
	fprintf(out, "BEGIN_SECTORS\n");
	while (fgets(line, sizeof(line), sidecar))
		fputs(line, out);
	if (line[0] && line[ft_strlen(line) - 1] != '\n')
		fputc('\n', out);
	fprintf(out, "END_SECTORS\n");
	fclose(sidecar);
	return (true);
}

bool	load_cub_text(char *src, char **lines, int *count)
{
	int		fd;
	char	*line;

	fd = open(src, O_RDONLY);
	if (fd < 0)
		return (false);
	line = get_next_line(fd, false);
	while (line && *count < DNK_MAX_LINES)
	{
		lines[(*count)++] = line;
		line = get_next_line(fd, false);
	}
	close(fd);
	free(line);
	return (*count > 0);
}

bool	write_all_assets(FILE *out, char *src)
{
	if (!write_cub_assets(out, src) || !write_asset_block(out,
			"elevator_button", ELEVATOR_BUTTON_ICON) || !write_sound_assets(out)
		|| !write_hud_assets(out))
		return (false);
	return (true);
}
