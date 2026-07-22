/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_level.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/22 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

typedef struct s_dnk_asset
{
	char	key[32];
	char	ext[8];
	char	path[LINE_SIZE];
}	t_dnk_asset;

typedef struct s_dnk
{
	char		*cub_lines[DNK_MAX_LINES];
	int			cub_count;
	char		*sector_lines[DNK_MAX_LINES];
	int			sector_count;
	t_dnk_asset	assets[DNK_MAX_ASSETS];
	int			asset_count;
	char		dir[LINE_SIZE];
	char		cub_path[LINE_SIZE];
}	t_dnk;

static void	trim_eol(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	while (len && (line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
}

static bool	starts_with(const char *text, const char *prefix)
{
	return (ft_strncmp(text, prefix, ft_strlen(prefix)) == 0);
}

static void	make_unpack_dir(t_dnk *dnk, t_game *g)
{
	char	sound_dir[LINE_SIZE];

	mkdir("build", 0775);
	snprintf(dnk->dir, LINE_SIZE, "%s_%d", DNK_UNPACK_ROOT, getpid());
	mkdir(dnk->dir, 0775);
	snprintf(sound_dir, LINE_SIZE, "%s/sounds", dnk->dir);
	mkdir(sound_dir, 0775);
	snprintf(dnk->cub_path, LINE_SIZE, "%s/level.cub", dnk->dir);
	ft_strlcpy(g->unpack_dir, dnk->dir, LINE_SIZE);
	g->unpacked_level = true;
}

static int	hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

static bool	write_hex_line(int fd, const char *line)
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
		low = hex_value(*line++);
		if (high < 0 || low < 0)
			return (false);
		byte = (unsigned char)((high << 4) | low);
		if (write(fd, &byte, 1) != 1)
			return (false);
	}
	return (true);
}

static bool	is_sound_asset(const char *key)
{
	return (starts_with(key, "sound_"));
}

static void	asset_path(t_dnk *dnk, t_dnk_asset *asset, t_game *g)
{
	if (is_sound_asset(asset->key))
	{
		snprintf(asset->path, LINE_SIZE, "%s/sounds/%s.%s", dnk->dir,
			asset->key + 6, asset->ext);
		snprintf(g->audio.sound_dir, LINE_SIZE, "%s/sounds/", dnk->dir);
	}
	else
		snprintf(asset->path, LINE_SIZE, "%s/%s.%s", dnk->dir,
			asset->key, asset->ext);
}

static bool	extract_asset(int fd, char *header, t_dnk *dnk, t_game *g)
{
	t_dnk_asset	*asset;
	char		*line;
	int			out;

	if (dnk->asset_count >= DNK_MAX_ASSETS)
		return (false);
	asset = &dnk->assets[dnk->asset_count];
	if (sscanf(header, "ASSET %31s %7s", asset->key, asset->ext) != 2)
		return (false);
	asset_path(dnk, asset, g);
	out = open(asset->path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (out < 0)
		return (false);
	line = get_next_line(fd, false);
	while (line)
	{
		trim_eol(line);
		if (ft_strcmp(line, "END_ASSET") == 0)
			break ;
		if (!write_hex_line(out, line))
			return (close(out), free(line), false);
		free(line);
		line = get_next_line(fd, false);
	}
	close(out);
	free(line);
	dnk->asset_count++;
	return (true);
}

static char	*dup_dnk_line(char *line, t_game *g)
{
	char	*copy;

	copy = ft_strdup(line);
	if (!copy)
		error("Unable to allocate packed level line", g);
	return (s_alloc(copy, g));
}

static bool	store_section_line(t_dnk *dnk, char *line, int state, t_game *g)
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

static bool	read_dnk(char *path, t_dnk *dnk, t_game *g)
{
	int		fd;
	int		state;
	char	*line;
	bool	ok;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	state = 0;
	ok = true;
	line = get_next_line(fd, false);
	while (line && ok)
	{
		trim_eol(line);
		if (ft_strcmp(line, DNK_MAGIC) == 0)
			ok = true;
		else if (ft_strcmp(line, "BEGIN_CUB") == 0)
			state = 1;
		else if (ft_strcmp(line, "END_CUB") == 0)
			state = 0;
		else if (ft_strcmp(line, "BEGIN_SECTORS") == 0)
			state = 2;
		else if (ft_strcmp(line, "END_SECTORS") == 0)
			state = 0;
		else if (starts_with(line, "ASSET "))
			ok = extract_asset(fd, line, dnk, g);
		else
			ok = store_section_line(dnk, line, state, g);
		free(line);
		line = get_next_line(fd, false);
	}
	close(fd);
	free(line);
	return (ok && dnk->cub_count > 0);
}

static const char	*find_asset_path(t_dnk *dnk, const char *key)
{
	int	i;

	i = 0;
	while (i < dnk->asset_count)
	{
		if (ft_strcmp((char *)key, dnk->assets[i].key) == 0)
			return (dnk->assets[i].path);
		i++;
	}
	return (NULL);
}

static void	append_replaced(char *dst, const char *src, t_dnk *dnk)
{
	char		key[32];
	int			i;
	const char	*asset;

	while (*src && ft_strlen(dst) < LINE_SIZE - 1)
	{
		if (*src != '@')
		{
			i = ft_strlen(dst);
			dst[i] = *src++;
			dst[i + 1] = '\0';
			continue ;
		}
		src++;
		i = 0;
		while ((ft_isalnum(*src) || *src == '_') && i < 31)
			key[i++] = *src++;
		key[i] = '\0';
		asset = find_asset_path(dnk, key);
		if (asset)
			ft_strlcat(dst, asset, LINE_SIZE);
		else
		{
			ft_strlcat(dst, "@", LINE_SIZE);
			ft_strlcat(dst, key, LINE_SIZE);
		}
	}
}

static bool	write_temp_cub(t_dnk *dnk)
{
	int		fd;
	int		i;
	char	line[LINE_SIZE];

	fd = open(dnk->cub_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (false);
	i = 0;
	while (i < dnk->cub_count)
	{
		line[0] = '\0';
		append_replaced(line, dnk->cub_lines[i], dnk);
		if (write(fd, line, ft_strlen(line)) < 0
			|| write(fd, "\n", 1) != 1)
			return (close(fd), false);
		i++;
	}
	close(fd);
	return (true);
}

static int	sector_id_from_char(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	return (0);
}

static void	apply_sector_grid_line(t_game *g, char *line, int y)
{
	int	x;

	x = 0;
	while (line[x] && x < g->map.width)
	{
		if (line[x] != ' ' && line[x] != '\t')
			set_sector_cell(g, x, y, sector_id_from_char(line[x]));
		x++;
	}
}

static void	parse_sector_line(t_game *g, char *line, int *grid_y)
{
	t_sector	sector;
	t_wall_segment	wall;
	int			id;
	int			transparent;

	if (starts_with(line, "SECTOR "))
	{
		sector.active = true;
		if (sscanf(line, "SECTOR %d %lf %lf %lf %lf %d", &id,
				&sector.floor_z, &sector.ceil_z, &sector.slope_x,
				&sector.slope_y, &sector.light) == 6)
			set_sector_info(g, id, sector);
	}
	else if (starts_with(line, "WALL "))
	{
		wall.texture = NORTH;
		wall.sector = 0;
		wall.transparent = false;
		transparent = 0;
		if (sscanf(line, "WALL %lf %lf %lf %lf %d %d %d",
				&wall.a.x, &wall.a.y, &wall.b.x, &wall.b.y,
				&wall.texture, &wall.sector, &transparent) >= 4)
		{
			wall.transparent = (transparent != 0);
			add_wall_segment(g, wall);
		}
	}
	else if (ft_strcmp(line, "GRID") == 0)
		*grid_y = 0;
	else if (*grid_y >= 0 && *grid_y < g->map.height)
		apply_sector_grid_line(g, line, (*grid_y)++);
}

static void	apply_packed_sectors(t_dnk *dnk, t_game *g)
{
	int	i;
	int	grid_y;

	init_default_sectors(g);
	i = 0;
	grid_y = -1;
	while (i < dnk->sector_count)
		parse_sector_line(g, dnk->sector_lines[i++], &grid_y);
}

int	ft_parse_packed_file(int argc, char *argv[], t_game *g)
{
	t_dnk	dnk;
	char	*cub_argv[2];

	(void)argc;
	ft_bzero(&dnk, sizeof(dnk));
	make_unpack_dir(&dnk, g);
	if (!read_dnk(argv[1], &dnk, g) || !write_temp_cub(&dnk))
		return (ft_parsing_error("Unable to unpack .dnk level.", 0));
	cub_argv[0] = argv[0];
	cub_argv[1] = dnk.cub_path;
	if (!ft_parse_file(2, cub_argv, g))
		return (0);
	apply_packed_sectors(&dnk, g);
	ft_strlcpy(g->level_source, argv[1], LINE_SIZE);
	return (1);
}

static bool	read_file_hex(FILE *out, const char *path)
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

static void	write_asset_block(FILE *out, const char *key, const char *path)
{
	const char	*ext;

	ext = ft_strrchr((char *)path, '.');
	if (!ext || !*(ext + 1))
		return ;
	fprintf(out, "ASSET %s %s\n", key, ext + 1);
	read_file_hex(out, path);
	fprintf(out, "END_ASSET\n");
}

static bool	header_asset_key(char *line, char *key, char *path)
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

static void	write_cub_line(FILE *out, char *line)
{
	char	key[32];
	char	path[LINE_SIZE];

	if (header_asset_key(line, key, path))
		fprintf(out, "%s @%s\n", key, key);
	else
		fprintf(out, "%s", line);
}

static void	write_sound_assets(FILE *out)
{
	DIR				*dir;
	struct dirent	*entry;
	char			path[LINE_SIZE];
	char			key[32];
	char			name[32];
	char			*dot;

	dir = opendir(SOUND_DIR);
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		dot = ft_strrchr(entry->d_name, '.');
		if (dot && (!ft_strcmp(dot, ".wav") || !ft_strcmp(dot, ".ogg")
				|| !ft_strcmp(dot, ".mp3")))
		{
			ft_strlcpy(key, "sound_", sizeof(key));
			ft_strlcpy(name, entry->d_name, sizeof(name));
			dot = ft_strrchr(name, '.');
			if (dot)
				*dot = '\0';
			ft_strlcat(key, name, sizeof(key));
			snprintf(path, LINE_SIZE, "%s%s", SOUND_DIR, entry->d_name);
			write_asset_block(out, key, path);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

static void	write_cub_assets(FILE *out, char *src)
{
	int		fd;
	char	*line;
	char	key[32];
	char	path[LINE_SIZE];

	fd = open(src, O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd, false);
	while (line)
	{
		if (header_asset_key(line, key, path))
			write_asset_block(out, key, path);
		free(line);
		line = get_next_line(fd, false);
	}
	close(fd);
}

static bool	is_map_source_line(char *line)
{
	int	i;
	int	has_map_char;

	i = 0;
	has_map_char = 0;
	while (line[i] && line[i] != '\n' && line[i] != '\r')
	{
		if (!ft_strchr("0123456789 NWSETHMX", line[i]))
			return (false);
		if (line[i] != ' ')
			has_map_char = 1;
		i++;
	}
	return (has_map_char);
}

static int	max_map_width(char **lines, int count)
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

static void	write_default_sector_grid(FILE *out, char **lines, int count)
{
	int	i;
	int	x;
	int	width;
	int	map_y;
	int	map_count;

	width = max_map_width(lines, count);
	map_count = 0;
	i = 0;
	while (i < count)
		if (is_map_source_line(lines[i++]))
			map_count++;
	fprintf(out, "BEGIN_SECTORS\n");
	fprintf(out, "SECTOR 0 0.00 1.00 0.00 0.00 255\n");
	fprintf(out, "SECTOR 1 0.35 1.35 0.03 0.00 190\n");
	fprintf(out, "GRID\n");
	i = 0;
	map_y = 0;
	while (i < count)
	{
		if (!is_map_source_line(lines[i++]))
			continue ;
		x = 0;
		while (x++ < width)
			fputc((map_y > map_count / 2) ? '1' : '0', out);
		fprintf(out, "\n");
		map_y++;
	}
	fprintf(out, "END_SECTORS\n");
}

static bool	load_cub_text(char *src, char **lines, int *count)
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

int	pack_level_file(char *src, char *dst)
{
	FILE	*out;
	char	*lines[DNK_MAX_LINES];
	int		count;
	int		i;

	count = 0;
	if (!load_cub_text(src, lines, &count))
		return (ft_parsing_error("Unable to read source level.", 0));
	out = fopen(dst, "w");
	if (!out)
		return (ft_parsing_error("Unable to write packed level.", 0));
	fprintf(out, "%s\n", DNK_MAGIC);
	write_cub_assets(out, src);
	write_sound_assets(out);
	fprintf(out, "BEGIN_CUB\n");
	i = 0;
	while (i < count)
		write_cub_line(out, lines[i++]);
	fprintf(out, "END_CUB\n");
	write_default_sector_grid(out, lines, count);
	fclose(out);
	i = 0;
	while (i < count)
		free(lines[i++]);
	return (1);
}
