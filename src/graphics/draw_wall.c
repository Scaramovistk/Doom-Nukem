/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:52 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:54 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static double	get_eye_z(t_player *p)
{
	return (p->z + p->eye_height);
}

void	draw_wall_slice(t_dimensions wall, t_ray *ray, t_game *g)
{
	t_texture_slice	slice;

	slice.screen_x = ray->x;
	slice.y_start = wall.top;
	slice.y_end = wall.bottom;
	slice.texture = get_wall_texture(ray, g);
	slice.texture_x = get_texture_x(ray, ray->distance, ray->side, g);
	slice.viewer_distance = ray->distance;
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
	slice.texture = &g->assets.textures[TRANSPARENT_T];
	slice.texture_x = get_texture_x(ray, hit->distance, hit->side, g);
	slice.viewer_distance = hit->distance;
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

void	draw_wall_decal(t_dimensions wall, t_ray *ray, t_game *g)
{
	t_texture_slice	slice;
	int				height;
	int				margin;

	if (!g->assets.textures[DECAL_T].img.ptr)
		return ;
	height = wall.bottom - wall.top;
	margin = height / 5;
	slice.screen_x = ray->x;
	slice.y_start = wall.top + margin;
	slice.y_end = wall.bottom - margin;
	slice.texture = &g->assets.textures[DECAL_T];
	slice.texture_x = get_texture_x(ray, ray->distance, ray->side, g);
	slice.viewer_distance = ray->distance;
	draw_texture_slice_alpha(&slice, g);
}

t_texture	*get_wall_texture(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			return (&g->assets.textures[EAST]);
		else
			return (&g->assets.textures[WEST]);
	}
	else
	{
		if (ray->dir.y > 0)
			return (&g->assets.textures[SOUTH]);
		else
			return (&g->assets.textures[NORTH]);
	}
}

void	get_wall_top_bottom(t_dimensions *wall, t_ray *ray, t_game *g)
{
	int	wall_height;
	int	horizon;
	double	eye_z;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	eye_z = get_eye_z(&g->player);
	wall_height = (int)(WIN_HEIGHT / ray->distance);
	wall->top = horizon - (int)((1.0 - eye_z) * wall_height);
	wall->bottom = horizon + (int)(eye_z * wall_height);
	if (wall->top < 0)
		wall->top = 0;
	if (wall->bottom > WIN_HEIGHT)
		wall->bottom = WIN_HEIGHT;
}
