/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	get_light_at_cell(t_game *g, t_coord cell)
{
	return (get_sector_at_cell(g, cell)->light);
}

int	get_light_at(t_game *g, t_position pos)
{
	t_coord	cell;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	return (get_light_at_cell(g, cell));
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
