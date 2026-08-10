/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   height_occlusion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	door_occludes_pixel(t_ray *ray, double depth, int y, t_game *g)
{
	t_dimensions	door;

	if (!ray->hit_door || depth <= ray->door_distance)
		return (false);
	get_door_top_bottom(&door, ray, g);
	return (y >= door.top && y < door.bottom);
}

static bool	z_range_contains_y(t_position range, double distance, int y,
		t_game *g)
{
	int	top;
	int	bottom;

	top = project_world_z(range.y, distance, g);
	bottom = project_world_z(range.x, distance, g);
	return (y >= top && y < bottom);
}

static bool	step_blocks_y(t_ray *ray, int i, int y, t_game *g)
{
	t_position	near;
	t_position	far;
	t_position	floor_range;
	t_position	ceiling_range;
	double		distance;

	distance = ray->height_steps[i].distance;
	near = ray_world_pos(ray, distance, g);
	far.x = near.x + ray->dir.x * 0.02;
	far.y = near.y + ray->dir.y * 0.02;
	floor_range.x = fmin(get_floor_z_at(g, near), get_floor_z_at(g, far));
	floor_range.y = fmax(get_floor_z_at(g, near), get_floor_z_at(g, far));
	ceiling_range.x = fmin(get_ceiling_z_at(g, near),
			get_ceiling_z_at(g, far));
	ceiling_range.y = fmax(get_ceiling_z_at(g, near),
			get_ceiling_z_at(g, far));
	return (z_range_contains_y(floor_range, distance, y, g)
		|| z_range_contains_y(ceiling_range, distance, y, g));
}

bool	height_step_occludes_pixel(t_ray *ray, double depth, int y, t_game *g)
{
	int	i;

	i = 0;
	while (i < ray->height_step_count)
	{
		if (depth > ray->height_steps[i].distance
			&& step_blocks_y(ray, i, y, g))
			return (true);
		i++;
	}
	return (false);
}
