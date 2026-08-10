/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_sector_apply.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	apply_sector_grid_line(t_game *g, char *line, int y)
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

bool	parse_sector_line(t_game *g, char *line, int *grid_y)
{
	if (starts_with(line, "ACTION "))
		return (add_authored_action(g, line));
	if (starts_with(line, "SECTOR "))
		return (parse_packed_sector(g, line));
	else if (starts_with(line, "WALL "))
		return (parse_packed_wall(g, line));
	else if (ft_strcmp(line, "GRID") == 0)
		*grid_y = 0;
	else if (*grid_y >= 0 && *grid_y < g->map.height)
		apply_sector_grid_line(g, line, (*grid_y)++);
	return (true);
}

bool	apply_packed_sectors(t_dnk *dnk, t_game *g)
{
	int	i;
	int	grid_y;

	init_default_sectors(g);
	i = 0;
	grid_y = -1;
	while (i < dnk->sector_count)
	{
		if (!parse_sector_line(g, dnk->sector_lines[i++], &grid_y))
			return (false);
	}
	return (true);
}

void	set_hud_source(char **field, const char *key, t_dnk *dnk, t_game *g)
{
	const char	*path;

	path = find_asset_path(dnk, key);
	if (path)
		*field = s_alloc(ft_strdup(path), g);
}

void	apply_packed_hud(t_dnk *dnk, t_game *g)
{
	set_hud_source(&g->assets.hud_weapons[0][0].source, "hud_pistol_idle", dnk,
		g);
	set_hud_source(&g->assets.hud_weapons[0][1].source, "hud_pistol_fire", dnk,
		g);
	set_hud_source(&g->assets.hud_weapons[1][0].source, "hud_blaster_idle", dnk,
		g);
	set_hud_source(&g->assets.hud_weapons[1][1].source, "hud_blaster_fire", dnk,
		g);
	set_hud_source(&g->assets.ammo_icon.source, "hud_ammo", dnk, g);
	set_hud_source(&g->assets.item_icons[0].source, "hud_item0", dnk, g);
	set_hud_source(&g->assets.item_icons[1].source, "hud_item1", dnk, g);
	set_hud_source(&g->assets.item_icons[2].source, "hud_item2", dnk, g);
	set_hud_source(&g->assets.item_icons[3].source, "hud_item3", dnk, g);
}
