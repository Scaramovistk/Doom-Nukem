/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_apply_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	append_asset_reference(char *dst, const char **src, t_dnk *dnk)
{
	char		key[32];
	int			i;
	const char	*asset;

	(*src)++;
	i = 0;
	while ((ft_isalnum(**src) || **src == '_') && i < 31)
		key[i++] = *(*src)++;
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

bool	parse_packed_sector(t_game *g, char *line)
{
	t_sector	sector;
	int			id;

	sector.active = true;
	sector.elevator_raised = false;
	if (sscanf(line, "SECTOR %d %lf %lf %lf %lf %d", &id,
			&sector.floor_z, &sector.ceil_z, &sector.slope_x, &sector.slope_y,
			&sector.light) == 6)
		set_sector_info(g, id, sector);
	return (true);
}

bool	parse_packed_wall(t_game *g, char *line)
{
	t_wall_segment	wall;
	int				transparent;

	wall.texture = NORTH;
	wall.sector = 0;
	wall.transparent = false;
	transparent = 0;
	if (sscanf(line, "WALL %lf %lf %lf %lf %d %d %d", &wall.a.x, &wall.a.y,
			&wall.b.x, &wall.b.y, &wall.texture, &wall.sector,
			&transparent) >= 4)
	{
		wall.transparent = (transparent != 0);
		add_wall_segment(g, wall);
	}
	return (true);
}
