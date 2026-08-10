/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_collision.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
	t_position	closest;
	double		ab_len;
	double		t;

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
