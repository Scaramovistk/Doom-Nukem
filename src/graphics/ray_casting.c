/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:13:10 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:13:16 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static double	cross(t_position a, t_position b)
{
	return (a.x * b.y - a.y * b.x);
}

static bool	segment_intersection(t_ray *ray, t_wall_segment *wall,
		t_game *g, double *distance)
{
	t_position	seg;
	t_position	to_wall;
	double		denom;
	double		t;
	double		u;

	seg.x = wall->b.x - wall->a.x;
	seg.y = wall->b.y - wall->a.y;
	denom = cross(ray->dir, seg);
	if (fabs(denom) < 0.000001)
		return (false);
	to_wall.x = wall->a.x - g->player.pos.x;
	to_wall.y = wall->a.y - g->player.pos.y;
	t = cross(to_wall, seg) / denom;
	u = cross(to_wall, ray->dir) / denom;
	if (t <= 0.01 || u < 0.0 || u > 1.0)
		return (false);
	*distance = t * cos(g->player.orientation - ray->angle);
	ray->segment_u = u;
	return (*distance > 0.0);
}

void	cast_segment_walls(t_ray *ray, t_game *g)
{
	int		i;
	double	distance;

	i = 0;
	while (i < g->map.segment_count)
	{
		if (segment_intersection(ray, &g->map.segments[i], g, &distance)
			&& distance < ray->distance)
		{
			ray->distance = distance;
			ray->segment_distance = distance;
			ray->segment_texture = g->map.segments[i].texture;
			ray->segment_sector = g->map.segments[i].sector;
			ray->hit_segment = true;
			ray->hit_block = WALL;
			ray->hit_door = NULL;
			ray->transparent_count = 0;
		}
		i++;
	}
}

void	cast_all_rays(t_ray *rays, t_game *g)
{
	double	start_angle;
	double	angle_step;
	int		i;
	double	ray_angle;

	start_angle = g->player.orientation - deg_to_rad(FOV / 2);
	angle_step = deg_to_rad(FOV) / WIN_WIDTH;
	i = 0;
	while (i < WIN_WIDTH)
	{
		ray_angle = start_angle + i * angle_step;
		cast_one_ray(&rays[i], ray_angle, g);
		i++;
	}
}

void	cast_one_ray(t_ray *ray, double angle, t_game *g)
{
	t_dda	dda;

	angle = normalize_angle(angle);
	ray->angle = angle;
	ray->dir.x = cos(angle);
	ray->dir.y = sin(angle);
	ray->hit_door = false;
	ray->hit_segment = false;
	ray->segment_u = 0.0;
	ray->segment_distance = 0.0;
	ray->segment_texture = NORTH;
	ray->segment_sector = 0;
	init_dda(&dda, ray, g);
	perform_dda(&dda, ray, g);
	cast_segment_walls(ray, g);
}
