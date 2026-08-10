/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_asset_write.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	apply_packed_defaults(t_dnk *dnk, t_game *g)
{
	const char	*path;

	path = find_asset_path(dnk, "elevator_button");
	if (path
		&& !g->assets.decoration_icons[ELEVATOR_BUTTON_DECORATION].source)
		g->assets.decoration_icons[ELEVATOR_BUTTON_DECORATION].source
			= s_alloc(ft_strdup(path), g);
}

int	ft_parse_packed_file(int argc, char *argv[], t_game *g)
{
	t_dnk	dnk;
	char	*cub_argv[2];

	(void)argc;
	ft_bzero(&dnk, sizeof(dnk));
	make_unpack_dir(&dnk, g);
	clear_packed_fallbacks(g);
	if (!read_dnk(argv[1], &dnk, g) || !packed_cub_is_self_contained(&dnk)
		|| !write_temp_cub(&dnk))
		return (ft_parsing_error("Unable to unpack .dnk level.", 0));
	cub_argv[0] = argv[0];
	cub_argv[1] = dnk.cub_path;
	if (!ft_parse_file(2, cub_argv, g))
		return (0);
	if (!apply_packed_sectors(&dnk, g))
		return (ft_parsing_error("Invalid authored action.", 0));
	apply_packed_defaults(&dnk, g);
	apply_packed_hud(&dnk, g);
	ft_strlcpy(g->level_source, argv[1], LINE_SIZE);
	return (1);
}

bool	read_file_hex(FILE *out, const char *path)
{
	int				fd;
	unsigned char	buffer[128];
	ssize_t			read_bytes;
	ssize_t			i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	read_bytes = read(fd, buffer, sizeof(buffer));
	while (read_bytes > 0)
	{
		i = 0;
		while (i < read_bytes)
			fprintf(out, "%02X", buffer[i++]);
		fprintf(out, "\n");
		read_bytes = read(fd, buffer, sizeof(buffer));
	}
	close(fd);
	return (true);
}

bool	write_asset_block(FILE *out, const char *key, const char *path)
{
	const char	*ext;
	int			fd;

	ext = ft_strrchr((char *)path, '.');
	if (!ext || !*(ext + 1))
		return (false);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	fprintf(out, "ASSET %s %s\n", key, ext + 1);
	if (!read_file_hex(out, path))
		return (false);
	fprintf(out, "END_ASSET\n");
	return (true);
}

bool	header_asset_key(char *line, char *key, char *path)
{
	char	prefix[8];
	char	value[LINE_SIZE];

	if (sscanf(line, "%7s %299s", prefix, value) != 2)
		return (false);
	if (!ft_xpm_extension(value))
		return (false);
	ft_strlcpy(path, value, LINE_SIZE);
	ft_strlcpy(key, prefix, 32);
	return (true);
}
