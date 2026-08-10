/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   document_sector_edit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	set_sector(t_editor_doc *doc, char *command)
{
	t_sector	sector;
	char		line[LINE_SIZE];
	int			id;

	if (!parse_sector_command(command, &sector, &id))
		return ((void) printf("Invalid sector values.\n"));
	snprintf(line, sizeof(line), "SECTOR %d %.3f %.3f %.4f %.4f %d", id,
		sector.floor_z, sector.ceil_z, sector.slope_x, sector.slope_y,
		sector.light);
	if (replace_sector_line(doc, line, id))
		return ;
	if (insert_line(doc->sectors, &doc->sector_count, doc->grid_start, line))
	{
		doc->grid_start++;
		doc->dirty = true;
	}
}

void	edit_sector_cell(t_editor_doc *doc, char *command)
{
	int		x;
	int		y;
	char	id;
	char	*row;

	if (sscanf(command, "cell %d %d %c", &x, &y, &id) != 3 || x < 0 || y < 0
		|| y >= doc->map_height || !((id >= '0' && id <= '9') || (id >= 'A'
				&& id <= 'Z')) || doc->grid_start + 1 + y >= doc->sector_count)
		return ((void) printf("Usage: cell <x> <y> <sector 0-9/A-Z>\n"));
	row = doc->sectors[doc->grid_start + 1 + y];
	if (x >= (int)ft_strlen(row))
		return ((void) printf("Sector row is narrower than the map.\n"));
	row[x] = id;
	doc->dirty = true;
}

void	add_wall(t_editor_doc *doc, char *command)
{
	t_wall_segment	wall;
	int				transparent;
	char			line[LINE_SIZE];

	if (sscanf(command, "wall add %lf %lf %lf %lf %d %d %d", &wall.a.x,
			&wall.a.y, &wall.b.x, &wall.b.y, &wall.texture, &wall.sector,
			&transparent) != 7 || !isfinite(wall.a.x) || !isfinite(wall.a.y)
		|| !isfinite(wall.b.x) || !isfinite(wall.b.y) || wall.texture < 0
		|| wall.texture >= TEXTURES_NB || wall.sector < 0
		|| wall.sector >= SECTOR_MAX
		|| (transparent != 0 && transparent != 1))
		return ((void) printf("Invalid wall values.\n"));
	snprintf(line, sizeof(line), "WALL %.3f %.3f %.3f %.3f %d %d %d",
		wall.a.x, wall.a.y, wall.b.x, wall.b.y, wall.texture, wall.sector,
		transparent != 0);
	if (insert_line(doc->sectors, &doc->sector_count, doc->grid_start, line))
	{
		doc->grid_start++;
		doc->dirty = true;
	}
}

void	clear_walls(t_editor_doc *doc)
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

void	add_action(t_editor_doc *doc, char *command)
{
	char	line[LINE_SIZE];

	if (snprintf(line, sizeof(line), "ACTION %s", command
			+ 11) >= (int) sizeof(line) || !authored_action_line_valid(line))
	{
		printf("Invalid action. Type help for the supported forms.\n");
		return ;
	}
	if (insert_line(doc->sectors, &doc->sector_count, doc->grid_start, line))
	{
		doc->grid_start++;
		doc->dirty = true;
	}
}
