/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   riser_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_z_range	riser_bounds(t_height_step *step, t_ray *ray, t_game *g,
		bool is_ceil)
{
	t_z_range	z;
	t_position	near;
	t_position	far;

	near = ray_world_pos(ray, step->distance, g);
	far.x = near.x + ray->dir.x * 0.02;
	far.y = near.y + ray->dir.y * 0.02;
	if (is_ceil)
	{
		z.lo = fmin(get_ceiling_z_at(g, near), get_ceiling_z_at(g, far));
		z.hi = fmax(get_ceiling_z_at(g, near), get_ceiling_z_at(g, far));
	}
	else
	{
		z.lo = fmin(get_floor_z_at(g, near), get_floor_z_at(g, far));
		z.hi = fmax(get_floor_z_at(g, near), get_floor_z_at(g, far));
	}
	return (z);
}

static void	draw_riser_slice(t_ray *ray, t_height_step *step, t_z_range z,
		t_game *g)
{
	t_texture_slice	slice;

	slice.screen_x = ray->x;
	slice.y_start = project_world_z(z.hi, step->distance, g);
	slice.raw_top = slice.y_start;
	slice.y_end = project_world_z(z.lo, step->distance, g);
	if (slice.y_start < 0)
		slice.y_start = 0;
	if (slice.y_end > WIN_HEIGHT)
		slice.y_end = WIN_HEIGHT;
	if (slice.y_start >= slice.y_end)
		return ;
	slice.texture = get_wall_texture(ray, g);
	slice.texture_x = get_texture_x(ray, step->distance, step->side, g);
	slice.viewer_distance = step->distance;
	slice.light = wall_light(step->side, get_light_at(g, ray_world_pos(ray,
					step->distance, g)), false);
	slice.ray = NULL;
	draw_texture_slice(&slice, g);
}

static void	draw_one_riser(t_height_step *step, t_ray *ray, t_game *g,
		bool is_ceil)
{
	int			save_side;
	double		save_distance;
	t_z_range	z;

	save_side = ray->side;
	save_distance = ray->distance;
	ray->side = step->side;
	ray->distance = step->distance;
	z = riser_bounds(step, ray, g, is_ceil);
	draw_riser_slice(ray, step, z, g);
	ray->side = save_side;
	ray->distance = save_distance;
}

static void	draw_band_pair(t_game *g, t_step_ctx ctx, int sector, double eye_z)
{
	ctx.is_floor = true;
	ctx.camera_height = g->map.sectors[sector].floor_z - eye_z;
	draw_step_band(ctx, g);
	if (g->assets.has_sky)
		return ;
	ctx.is_floor = false;
	ctx.camera_height = g->map.sectors[sector].ceil_z - eye_z;
	draw_step_band(ctx, g);
}

void	draw_height_steps(t_ray *ray, t_game *g)
{
	t_step_ctx	ctx;
	int			i;
	double		eye_z;

	if (ray->height_step_count == 0)
		return ;
	eye_z = g->player.z + g->player.eye_height;
	ctx.ray = ray;
	ctx.inv_cos = 1.0 / cos(g->player.orientation - ray->angle);
	i = ray->height_step_count;
	while (i >= 0)
	{
		ctx.far_d = step_far_d(ray, i);
		if (i == 0)
			ctx.near_d = 0.0;
		else
			ctx.near_d = ray->height_steps[i - 1].distance;
		draw_band_pair(g, ctx, band_sector(ray, i), eye_z);
		if (i > 0)
		{
			draw_one_riser(&ray->height_steps[i - 1], ray, g, false);
			draw_one_riser(&ray->height_steps[i - 1], ray, g, true);
		}
		i--;
	}
}
