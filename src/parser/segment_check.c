/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	points_match(t_position a, t_position b)
{
	return (fabs(a.x - b.x) < SEGMENT_EPSILON && fabs(a.y
			- b.y) < SEGMENT_EPSILON);
}

static bool	endpoint_connected(t_game *g, t_position point, int self)
{
	int	i;

	i = 0;
	while (i < g->map.segment_count)
	{
		if (i != self && (points_match(point, g->map.segments[i].a)
				|| points_match(point, g->map.segments[i].b)))
			return (true);
		i++;
	}
	return (false);
}

void	check_segment_loops(t_game *g)
{
	int	i;
	int	open_count;

	open_count = 0;
	i = 0;
	while (i < g->map.segment_count)
	{
		if (!endpoint_connected(g, g->map.segments[i].a, i)
			|| !endpoint_connected(g, g->map.segments[i].b, i))
			open_count++;
		i++;
	}
	if (open_count > 0)
		fprintf(stderr,
			"Warning: %d wall segment(s) do not close into a loop\n",
			open_count);
}
