/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	advance_line(t_coord *a, t_coord delta, t_coord step, int *err)
{
	int	err2;

	err2 = 2 * (*err);
	if (err2 >= delta.y)
	{
		*err += delta.y;
		a->x += step.x;
	}
	if (err2 <= delta.x)
	{
		*err += delta.x;
		a->y += step.y;
	}
}

static void	draw_line(t_game *g, t_coord a, t_coord b, int color)
{
	t_coord	delta;
	t_coord	step;
	int		err;

	delta.x = abs(b.x - a.x);
	delta.y = -abs(b.y - a.y);
	step.x = -1 + 2 * (a.x < b.x);
	step.y = -1 + 2 * (a.y < b.y);
	err = delta.x + delta.y;
	while (true)
	{
		minimap_pixel(g, a.x, a.y, color);
		if (a.x == b.x && a.y == b.y)
			break ;
		advance_line(&a, delta, step, &err);
	}
}

void	draw_map_segments(t_game *g, t_coord origin)
{
	int		i;
	t_coord	a;
	t_coord	b;

	i = 0;
	while (i < g->map.segment_count)
	{
		a.x = origin.x + (int)(g->map.segments[i].a.x * MAP_SCALE);
		a.y = origin.y + (int)(g->map.segments[i].a.y * MAP_SCALE);
		b.x = origin.x + (int)(g->map.segments[i].b.x * MAP_SCALE);
		b.y = origin.y + (int)(g->map.segments[i].b.y * MAP_SCALE);
		draw_line(g, a, b, YELLOW);
		i++;
	}
}
