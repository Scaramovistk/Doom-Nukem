/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_slice.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_position	ray_world_pos(t_ray *ray, double distance, t_game *g)
{
	t_position	pos;
	double		true_distance;

	true_distance = distance / cos(g->player.orientation - ray->angle);
	pos.x = g->player.pos.x + true_distance * ray->dir.x;
	pos.y = g->player.pos.y + true_distance * ray->dir.y;
	pos.x -= ray->dir.x * 0.01;
	pos.y -= ray->dir.y * 0.01;
	return (pos);
}

int	project_world_z(double world_z, double distance, t_game *g)
{
	int		horizon;
	double	eye_z;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	eye_z = g->player.z + g->player.eye_height;
	return (horizon - (int)((world_z - eye_z) * WIN_HEIGHT / distance));
}

void	draw_wall_slice(t_dimensions wall, t_ray *ray, t_game *g)
{
	t_texture_slice	slice;

	slice.screen_x = ray->x;
	slice.y_start = wall.top;
	slice.y_end = wall.bottom;
	slice.raw_top = wall.raw_top;
	slice.texture = get_wall_texture(ray, g);
	slice.texture_x = get_texture_x(ray, ray->distance, ray->side, g);
	slice.viewer_distance = ray->distance;
	slice.light = wall_light(ray->side, get_light_at(g, ray_world_pos(ray,
					ray->distance, g)), ray->hit_segment);
	slice.ray = ray;
	draw_texture_slice(&slice, g);
}

static void	draw_transparent_hit(t_transparent_hit *hit, t_ray *ray, t_game *g)
{
	t_texture_slice	slice;
	t_dimensions	wall;
	double			save_distance;
	int				save_side;

	save_distance = ray->distance;
	save_side = ray->side;
	ray->distance = hit->distance;
	ray->side = hit->side;
	get_wall_top_bottom(&wall, ray, g);
	slice.screen_x = ray->x;
	slice.y_start = wall.top;
	slice.y_end = wall.bottom;
	slice.raw_top = wall.raw_top;
	slice.texture = &g->assets.textures[TRANSPARENT_T];
	slice.texture_x = get_texture_x(ray, hit->distance, hit->side, g);
	slice.viewer_distance = hit->distance;
	slice.light = wall_light(hit->side, get_light_at(g, ray_world_pos(ray,
					hit->distance, g)), false);
	slice.ray = ray;
	draw_texture_slice_alpha(&slice, g);
	ray->distance = save_distance;
	ray->side = save_side;
}

void	draw_transparent_walls(t_ray *ray, t_game *g)
{
	int	i;

	if (!g->assets.textures[TRANSPARENT_T].img.ptr)
		return ;
	i = ray->transparent_count;
	while (i--)
		draw_transparent_hit(&ray->transparent_hits[i], ray, g);
}
