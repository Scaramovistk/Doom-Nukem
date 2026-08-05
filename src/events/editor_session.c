/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_session.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/04 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

typedef struct s_editor_doc
{
	char	*cub[DNK_MAX_LINES];
	int		cub_count;
	int		map_start;
	int		map_height;
	int		map_width;
	char	*sectors[DNK_MAX_LINES];
	int		sector_count;
	int		grid_start;
	char	cub_path[LINE_SIZE];
	char	sector_path[LINE_SIZE];
	char	output_path[LINE_SIZE];
	bool	dirty;
} 			t_editor_doc;

static char	*editor_dup(const char *text)
{
	char	*copy;
	size_t	len;

	copy = ft_strdup(text);
	if (!copy)
		return (NULL);
	len = ft_strlen(copy);
	while (len && (copy[len - 1] == '\n' || copy[len - 1] == '\r'))
		copy[--len] = '\0';
	return (copy);
}

static void	free_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
}

static void	free_document(t_editor_doc *doc)
{
	free_lines(doc->cub, doc->cub_count);
	free_lines(doc->sectors, doc->sector_count);
	doc->cub_count = 0;
	doc->sector_count = 0;
}

static bool	read_lines(const char *path, char **lines, int *count)
{
	char	*line;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	line = get_next_line(fd, false);
	while (line && *count < DNK_MAX_LINES)
	{
		lines[*count] = editor_dup(line);
		free(line);
		if (!lines[*count])
			return (close(fd), false);
		(*count)++;
		line = get_next_line(fd, false);
	}
	free(line);
	close(fd);
	return (*count > 0);
}

static bool	is_map_line(const char *line)
{
	const char	*valid;
	int			i;
	bool		non_space;

	valid = "0123456789 NWSETHMXKLPIDCVBabcdefv";
	i = 0;
	non_space = false;
	while (line[i])
	{
		if (!ft_strchr(valid, line[i]))
			return (false);
		if (line[i] != ' ')
			non_space = true;
		i++;
	}
	return (non_space);
}

static bool	scan_map(t_editor_doc *doc)
{
	int	i;
	int	width;

	doc->map_start = -1;
	doc->map_height = 0;
	doc->map_width = 0;
	i = 0;
	while (i < doc->cub_count && !is_map_line(doc->cub[i]))
		i++;
	if (i == doc->cub_count)
		return (false);
	doc->map_start = i;
	while (i < doc->cub_count && is_map_line(doc->cub[i]))
	{
		width = (int)ft_strlen(doc->cub[i]);
		if (width > doc->map_width)
			doc->map_width = width;
		doc->map_height++;
		i++;
	}
	return (true);
}

static void	make_sector_path(t_editor_doc *doc)
{
	char	*extension;

	ft_strlcpy(doc->sector_path, doc->cub_path, LINE_SIZE);
	extension = ft_strrchr(doc->sector_path, '.');
	if (extension && !ft_strcmp(extension, ".cub"))
		ft_strlcpy(extension, ".sectors",
			LINE_SIZE - (extension - doc->sector_path));
	else
		ft_strlcat(doc->sector_path, ".sectors", LINE_SIZE);
}

static bool	append_line(char **lines, int *count, const char *text)
{
	if (*count >= DNK_MAX_LINES)
		return (false);
	lines[*count] = editor_dup(text);
	if (!lines[*count])
		return (false);
	(*count)++;
	return (true);
}

static void	scan_sector_grid(t_editor_doc *doc)
{
	int	i;

	doc->grid_start = -1;
	i = 0;
	while (i < doc->sector_count)
	{
		if (!ft_strcmp(doc->sectors[i], "GRID"))
			doc->grid_start = i;
		i++;
	}
}

static bool	default_sectors(t_editor_doc *doc)
{
	char	*row;
	int		y;

	if (!append_line(doc->sectors, &doc->sector_count,
			"SECTOR 0 0.00 1.00 0.00 0.00 255")
		|| !append_line(doc->sectors, &doc->sector_count, "GRID"))
		return (false);
	row = malloc(doc->map_width + 1);
	if (!row)
		return (false);
	ft_memset(row, '0', doc->map_width);
	row[doc->map_width] = '\0';
	y = 0;
	while (y++ < doc->map_height)
		if (!append_line(doc->sectors, &doc->sector_count, row))
			return (free(row), false);
	free(row);
	scan_sector_grid(doc);
	return (true);
}

static bool	load_document(t_editor_doc *doc, char *src, char *output)
{
	ft_bzero(doc, sizeof(*doc));
	ft_strlcpy(doc->cub_path, src, LINE_SIZE);
	ft_strlcpy(doc->output_path, output, LINE_SIZE);
	if (!read_lines(doc->cub_path, doc->cub, &doc->cub_count)
		|| !scan_map(doc))
		return (free_document(doc), false);
	make_sector_path(doc);
	read_lines(doc->sector_path, doc->sectors, &doc->sector_count);
	if (!doc->sector_count && !default_sectors(doc))
		return (free_document(doc), false);
	scan_sector_grid(doc);
	if (doc->grid_start < 0)
		return (free_document(doc), false);
	return (true);
}

static bool	write_lines(const char *path, char **lines, int count)
{
	char	tmp[LINE_SIZE];
	FILE	*out;
	int		i;

	if (snprintf(tmp, sizeof(tmp), "%s.tmp.%d", path, getpid()) >= (int)sizeof(tmp))
		return (false);
	out = fopen(tmp, "w");
	if (!out)
		return (false);
	i = 0;
	while (i < count)
	{
		if (fprintf(out, "%s\n", lines[i++]) < 0)
			return (fclose(out), unlink(tmp), false);
	}
	if (fclose(out) != 0 || rename(tmp, path) != 0)
		return (unlink(tmp), false);
	return (true);
}

static bool	save_document(t_editor_doc *doc)
{
	if (!write_lines(doc->cub_path, doc->cub, doc->cub_count)
		|| !write_lines(doc->sector_path, doc->sectors, doc->sector_count))
		return (printf("Save failed.\n"), false);
	doc->dirty = false;
	printf("Saved %s and %s\n", doc->cub_path, doc->sector_path);
	return (true);
}

static bool	valid_map_token(char token)
{
	return (ft_strchr("0123456789NWSETHMXKLPIDCVBabcdefv", token) != NULL);
}

static void	set_map_cell(t_editor_doc *doc, char *command)
{
	int		x;
	int		y;
	char	token;

	if (sscanf(command, "set %d %d %c", &x, &y, &token) != 3
		|| x < 0 || y < 0 || y >= doc->map_height
		|| x >= (int)ft_strlen(doc->cub[doc->map_start + y])
		|| !valid_map_token(token))
		return ((void)printf("Usage: set <x> <y> <map-token>\n"));
	doc->cub[doc->map_start + y][x] = token;
	doc->dirty = true;
}

static bool	line_has_key(const char *line, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (!ft_strncmp(line, key, len)
		&& (line[len] == ' ' || line[len] == '\t'));
}

static bool	insert_line(char **lines, int *count, int index, const char *text)
{
	int	i;

	if (*count >= DNK_MAX_LINES || index < 0 || index > *count)
		return (false);
	i = *count;
	while (i > index)
	{
		lines[i] = lines[i - 1];
		i--;
	}
	lines[index] = editor_dup(text);
	if (!lines[index])
		return (false);
	(*count)++;
	return (true);
}

static void	set_texture(t_editor_doc *doc, char *command)
{
	char	key[16];
	char	value[LINE_SIZE];
	char	line[LINE_SIZE];
	int		i;

	if (sscanf(command, "texture %15s %299[^\n]", key, value) != 2)
		return ((void)printf("Usage: texture <header-key> <path-or-RGB>\n"));
	if (snprintf(line, sizeof(line), "%s %s", key, value) >= (int)sizeof(line))
		return ((void)printf("Texture line is too long.\n"));
	i = 0;
	while (i < doc->map_start)
	{
		if (line_has_key(doc->cub[i], key))
		{
			free(doc->cub[i]);
			doc->cub[i] = editor_dup(line);
			doc->dirty = true;
			return ;
		}
		i++;
	}
	if (!insert_line(doc->cub, &doc->cub_count, doc->map_start, line))
		return ((void)printf("Unable to add header line.\n"));
	doc->map_start++;
	doc->dirty = true;
}

static void	set_sector(t_editor_doc *doc, char *command)
{
	char	line[LINE_SIZE];
	double	floor_z;
	double	ceil_z;
	double	sx;
	double	sy;
	int		id;
	int		light;
	int		i;
	int		line_id;

	if (sscanf(command, "sector %d %lf %lf %lf %lf %d", &id, &floor_z,
			&ceil_z, &sx, &sy, &light) != 6 || id < 0 || id >= SECTOR_MAX
		|| ceil_z <= floor_z || light < 0 || light > 255)
		return ((void)printf("Usage: sector <id> <floor> <ceil> <sx> <sy> <light>\n"));
	snprintf(line, sizeof(line), "SECTOR %d %.3f %.3f %.4f %.4f %d",
		id, floor_z, ceil_z, sx, sy, light);
	i = 0;
	while (i < doc->sector_count)
	{
		if (sscanf(doc->sectors[i], "SECTOR %d", &line_id) == 1
			&& line_id == id)
		{
			free(doc->sectors[i]);
			doc->sectors[i] = editor_dup(line);
			doc->dirty = true;
			return ;
		}
		i++;
	}
	if (insert_line(doc->sectors, &doc->sector_count, doc->grid_start, line))
	{
		doc->grid_start++;
		doc->dirty = true;
	}
}

static void	edit_sector_cell(t_editor_doc *doc, char *command)
{
	int		x;
	int		y;
	char	id;
	char	*row;

	if (sscanf(command, "cell %d %d %c", &x, &y, &id) != 3
		|| x < 0 || y < 0 || y >= doc->map_height
		|| !((id >= '0' && id <= '9') || (id >= 'A' && id <= 'Z'))
		|| doc->grid_start + 1 + y >= doc->sector_count)
		return ((void)printf("Usage: cell <x> <y> <sector 0-9/A-Z>\n"));
	row = doc->sectors[doc->grid_start + 1 + y];
	if (x >= (int)ft_strlen(row))
		return ((void)printf("Sector row is narrower than the map.\n"));
	row[x] = id;
	doc->dirty = true;
}

static void	add_wall(t_editor_doc *doc, char *command)
{
	double	x1;
	double	y1;
	double	x2;
	double	y2;
	int		texture;
	int		sector;
	int		transparent;
	char	line[LINE_SIZE];

	if (sscanf(command, "wall add %lf %lf %lf %lf %d %d %d", &x1, &y1,
			&x2, &y2, &texture, &sector, &transparent) != 7)
		return ((void)printf("Usage: wall add <x1> <y1> <x2> <y2> <texture> <sector> <0|1>\n"));
	snprintf(line, sizeof(line), "WALL %.3f %.3f %.3f %.3f %d %d %d",
		x1, y1, x2, y2, texture, sector, transparent != 0);
	if (insert_line(doc->sectors, &doc->sector_count, doc->grid_start, line))
	{
		doc->grid_start++;
		doc->dirty = true;
	}
}

static void	clear_walls(t_editor_doc *doc)
{
	int	i;

	i = 0;
	while (i < doc->sector_count)
	{
		if (!ft_strncmp(doc->sectors[i], "WALL ", 5))
		{
			free(doc->sectors[i]);
			doc->sector_count--;
			while (i < doc->sector_count)
			{
				doc->sectors[i] = doc->sectors[i + 1];
				i++;
			}
			doc->dirty = true;
			scan_sector_grid(doc);
			return (clear_walls(doc));
		}
		i++;
	}
}

static void	show_map(t_editor_doc *doc)
{
	int	y;

	printf("Map %dx%d (x across, y down):\n", doc->map_width, doc->map_height);
	y = 0;
	while (y < doc->map_height)
	{
		printf("%3d %s\n", y, doc->cub[doc->map_start + y]);
		y++;
	}
}

static void	show_sectors(t_editor_doc *doc)
{
	int	i;

	i = 0;
	while (i < doc->sector_count)
	{
		printf("%3d %s\n", i, doc->sectors[i]);
		i++;
	}
}

static void	print_help(void)
{
	printf("Editor commands:\n");
	printf("  show map | show sectors\n");
	printf("  set X Y TOKEN              place geometry/device/entity\n");
	printf("  texture KEY PATH_OR_RGB    set any cub header texture/value\n");
	printf("  sector ID FLOOR CEIL SX SY LIGHT\n");
	printf("  cell X Y ID                assign sector grid cell\n");
	printf("  wall add X1 Y1 X2 Y2 TEXTURE SECTOR TRANSPARENT\n");
	printf("  wall clear\n");
	printf("  save | validate | pack [output.dnk] | quit\n");
	printf("Map tokens: 0 floor, 1 wall, 2 door, B locked, P secret,\n");
	printf(" L elevator, T timed switch, H hazard, M message, X exit,\n");
	printf(" 6 health, 7 ammo, 8 key, 9 jetpack, 3/K/I/D/C enemies,\n");
	printf(" V solid object, v pass-through object, a-f decorations.\n");
}

static void	validate_document(t_editor_doc *doc)
{
	char	tmp[LINE_SIZE];

	if (!save_document(doc))
		return ;
	snprintf(tmp, sizeof(tmp), "/tmp/doom_nukem_editor_%d.dnk", getpid());
	if (!pack_level_file(doc->cub_path, tmp))
		return ((void)printf("Validation pack failed.\n"));
	check_level_file(tmp);
	unlink(tmp);
}

static void	pack_document(t_editor_doc *doc, char *command)
{
	char	output[LINE_SIZE];

	if (sscanf(command, "pack %299s", output) != 1)
		ft_strlcpy(output, doc->output_path, LINE_SIZE);
	if (!ft_dnk_extension(output))
		return ((void)printf("Pack output must end in .dnk\n"));
	if (save_document(doc) && pack_level_file(doc->cub_path, output))
		printf("Packed %s\n", output);
}

static bool	run_command(t_editor_doc *doc, char *command)
{
	size_t	len;

	len = ft_strlen(command);
	while (len && (command[len - 1] == '\n' || command[len - 1] == '\r'))
		command[--len] = '\0';
	if (!ft_strcmp(command, "quit") || !ft_strcmp(command, "exit"))
		return (false);
	if (!ft_strcmp(command, "help"))
		print_help();
	else if (!ft_strcmp(command, "show map"))
		show_map(doc);
	else if (!ft_strcmp(command, "show sectors"))
		show_sectors(doc);
	else if (!ft_strncmp(command, "set ", 4))
		set_map_cell(doc, command);
	else if (!ft_strncmp(command, "texture ", 8))
		set_texture(doc, command);
	else if (!ft_strncmp(command, "sector ", 7))
		set_sector(doc, command);
	else if (!ft_strncmp(command, "cell ", 5))
		edit_sector_cell(doc, command);
	else if (!ft_strncmp(command, "wall add ", 9))
		add_wall(doc, command);
	else if (!ft_strcmp(command, "wall clear"))
		clear_walls(doc);
	else if (!ft_strcmp(command, "save"))
		save_document(doc);
	else if (!ft_strcmp(command, "validate"))
		validate_document(doc);
	else if (!ft_strncmp(command, "pack", 4))
		pack_document(doc, command);
	else if (command[0])
		printf("Unknown command. Type help.\n");
	return (true);
}

int	interactive_level_editor(char *src, char *default_output)
{
	t_editor_doc	doc;
	char			command[LINE_SIZE];
	bool			running;

	if (!load_document(&doc, src, default_output))
		return (printf("Unable to load editable .cub/.sectors project.\n"),
			EXIT_FAILURE);
	printf("Doom-Nukem interactive level editor: %s\n", src);
	print_help();
	running = true;
	while (running)
	{
		if (isatty(STDIN_FILENO))
			printf("dnk-edit> ");
		if (!fgets(command, sizeof(command), stdin))
			break ;
		running = run_command(&doc, command);
	}
	if (doc.dirty)
		printf("Unsaved changes discarded (use 'save' or 'pack').\n");
	free_document(&doc);
	return (EXIT_SUCCESS);
}
