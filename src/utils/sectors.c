/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sectors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/22 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_sector	default_sector(void)
{
	t_sector	sector;

	sector.floor_z = PLAYER_FLOOR_Z;
	sector.ceil_z = 1.0;
	sector.slope_x = 0.0;
	sector.slope_y = 0.0;
	sector.light = DNK_DEFAULT_LIGHT;
	sector.active = true;
	return (sector);
}

void	init_default_sectors(t_game *g)
{
	int	y;
	int	x;

	g->map.sector_count = 1;
	g->map.sectors[0] = default_sector();
	if (!g->map.width || !g->map.height || g->map.sector_grid)
		return ;
	g->map.sector_grid = calloc_s(g->map.height, sizeof(int *), g);
	y = 0;
	while (y < g->map.height)
	{
		g->map.sector_grid[y] = calloc_s(g->map.width, sizeof(int), g);
		x = 0;
		while (x < g->map.width)
			g->map.sector_grid[y][x++] = 0;
		y++;
	}
}

bool	set_sector_info(t_game *g, int id, t_sector sector)
{
	if (id < 0 || id >= SECTOR_MAX)
		return (false);
	if (sector.ceil_z <= sector.floor_z + 0.25)
		sector.ceil_z = sector.floor_z + 0.25;
	if (sector.light < 0)
		sector.light = 0;
	if (sector.light > 255)
		sector.light = 255;
	sector.active = true;
	g->map.sectors[id] = sector;
	if (id >= g->map.sector_count)
		g->map.sector_count = id + 1;
	return (true);
}

bool	set_sector_cell(t_game *g, int x, int y, int sector)
{
	if (!g->map.sector_grid || x < 0 || y < 0
		|| x >= g->map.width || y >= g->map.height
		|| sector < 0 || sector >= SECTOR_MAX)
		return (false);
	g->map.sector_grid[y][x] = sector;
	return (true);
}

static t_sector	*sector_at_cell(t_game *g, t_coord cell)
{
	int	id;

	if (!g->map.sector_count)
		init_default_sectors(g);
	id = 0;
	if (g->map.sector_grid && cell.x >= 0 && cell.y >= 0
		&& cell.x < g->map.width && cell.y < g->map.height)
		id = g->map.sector_grid[cell.y][cell.x];
	if (id < 0 || id >= SECTOR_MAX || !g->map.sectors[id].active)
		id = 0;
	return (&g->map.sectors[id]);
}

static t_coord	cell_from_pos(t_position pos)
{
	t_coord	cell;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	return (cell);
}

double	get_floor_z_at(t_game *g, t_position pos)
{
	t_coord		cell;
	t_sector	*sector;
	double		local_x;
	double		local_y;

	cell = cell_from_pos(pos);
	sector = sector_at_cell(g, cell);
	local_x = pos.x - cell.x - 0.5;
	local_y = pos.y - cell.y - 0.5;
	return (sector->floor_z + sector->slope_x * local_x
		+ sector->slope_y * local_y);
}

double	get_ceiling_z_at(t_game *g, t_position pos)
{
	t_coord		cell;
	t_sector	*sector;
	double		local_x;
	double		local_y;
	double		ceil_z;
	double		floor_z;

	cell = cell_from_pos(pos);
	sector = sector_at_cell(g, cell);
	local_x = pos.x - cell.x - 0.5;
	local_y = pos.y - cell.y - 0.5;
	ceil_z = sector->ceil_z + sector->slope_x * local_x
		+ sector->slope_y * local_y;
	floor_z = get_floor_z_at(g, pos);
	if (ceil_z < floor_z + 0.25)
		ceil_z = floor_z + 0.25;
	return (ceil_z);
}

double	get_floor_z_at_cell(t_game *g, t_coord cell)
{
	t_position	pos;

	pos.x = cell.x + 0.5;
	pos.y = cell.y + 0.5;
	return (get_floor_z_at(g, pos));
}

int	get_light_at_cell(t_game *g, t_coord cell)
{
	return (sector_at_cell(g, cell)->light);
}

int	get_light_at(t_game *g, t_position pos)
{
	return (get_light_at_cell(g, cell_from_pos(pos)));
}

int	apply_light(int color, int light, double distance)
{
	double	scale;
	int		red;
	int		green;
	int		blue;

	if ((color & 0x00FFFFFF) == 0x00FF00FF)
		return (color);
	scale = (double)light / 255.0;
	scale *= 1.0 / (1.0 + distance * 0.035);
	if (scale < 0.16)
		scale = 0.16;
	if (scale > 1.0)
		scale = 1.0;
	red = ((color >> 16) & 0xFF) * scale;
	green = ((color >> 8) & 0xFF) * scale;
	blue = (color & 0xFF) * scale;
	return ((red << 16) | (green << 8) | blue);
}

bool	add_wall_segment(t_game *g, t_wall_segment segment)
{
	if (g->map.segment_count >= SEGMENT_WALL_MAX)
		return (false);
	if (segment.texture < 0 || segment.texture >= TEXTURES_NB)
		segment.texture = NORTH;
	if (segment.sector < 0 || segment.sector >= SECTOR_MAX)
		segment.sector = 0;
	g->map.segments[g->map.segment_count++] = segment;
	return (true);
}

static double	point_segment_distance(t_position p, t_wall_segment *wall)
{
	t_position	ab;
	t_position	ap;
	double		ab_len;
	double		t;
	t_position	closest;

	ab.x = wall->b.x - wall->a.x;
	ab.y = wall->b.y - wall->a.y;
	ap.x = p.x - wall->a.x;
	ap.y = p.y - wall->a.y;
	ab_len = ab.x * ab.x + ab.y * ab.y;
	if (ab_len <= 0.000001)
		return (sqrt(ap.x * ap.x + ap.y * ap.y));
	t = (ap.x * ab.x + ap.y * ab.y) / ab_len;
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	closest.x = wall->a.x + ab.x * t;
	closest.y = wall->a.y + ab.y * t;
	return (sqrt(pow(p.x - closest.x, 2) + pow(p.y - closest.y, 2)));
}

bool	segment_blocks_position(t_game *g, t_position pos)
{
	int	i;

	i = 0;
	while (i < g->map.segment_count)
	{
		if (point_segment_distance(pos, &g->map.segments[i])
			< COLLISION_SAFETY * 1.4)
			return (true);
		i++;
	}
	return (false);
}
