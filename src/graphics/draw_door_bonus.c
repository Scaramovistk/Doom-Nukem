/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:40 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:42 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_position	door_world_pos(t_ray *ray, t_game *g)
{
	t_position	pos;
	double		true_distance;

	true_distance = ray->door_distance
		/ cos(g->player.orientation - ray->angle);
	pos.x = g->player.pos.x + true_distance * ray->dir.x;
	pos.y = g->player.pos.y + true_distance * ray->dir.y;
	pos.x -= ray->dir.x * 0.01;
	pos.y -= ray->dir.y * 0.01;
	return (pos);
}

static t_texture	*door_texture(t_ray *ray, t_game *g)
{
	int			texture;
	int			i;
	bool		secret;

	secret = false;
	i = 0;
	while (i < g->map.secret_count)
	{
		if (&g->map.doors[g->map.secrets[i].y][g->map.secrets[i].x]
			== ray->hit_door)
			secret = true;
		i++;
	}
	if (!secret)
		return (&g->assets.textures[DOOR_T]);
	if (ray->door_side == 0)
	{
		texture = EAST;
		if (ray->dir.x < 0)
			texture = WEST;
	}
	else
	{
		texture = SOUTH;
		if (ray->dir.y < 0)
			texture = NORTH;
	}
	return (&g->assets.textures[texture]);
}

void	draw_door_slice(t_ray *ray, t_game *g)
{
	t_texture_slice	slice;
	int				door_top;
	int				door_bottom;
	int				raw_top;

	get_door_top_bottom(&door_top, &door_bottom, &raw_top, ray, g);
	slice.screen_x = ray->x;
	slice.y_start = door_top;
	slice.y_end = door_bottom;
	slice.raw_top = raw_top;
	slice.texture = door_texture(ray, g);
	slice.texture_x = get_texture_x(ray, ray->door_distance, ray->door_side, g);
	slice.viewer_distance = ray->door_distance;
	slice.light = get_light_at(g, door_world_pos(ray, g));
	slice.ray = ray;
	draw_texture_slice(&slice, g);
}

void	get_door_top_bottom(int *door_top, int *door_bottom,
		int *raw_top, t_ray *ray, t_game *g)
{
	int	visible_door_height;
	int	full_top;
	int	full_bottom;
	t_position	pos;
	double	floor_z;
	double	ceil_z;

	pos = door_world_pos(ray, g);
	floor_z = get_floor_z_at(g, pos);
	ceil_z = get_ceiling_z_at(g, pos);
	full_top = project_world_z(ceil_z, ray->door_distance, g);
	full_bottom = project_world_z(floor_z, ray->door_distance, g);
	visible_door_height = (full_bottom - full_top) * (1.0
			- ray->hit_door->opening_state);
	*door_bottom = full_bottom;
	*door_top = *door_bottom - visible_door_height;
	*raw_top = full_top;
	if (*door_top < 0)
		*door_top = 0;
	if (*door_bottom > WIN_HEIGHT)
		*door_bottom = WIN_HEIGHT;
}

bool	door_occludes_pixel(t_ray *ray, double depth, int y, t_game *g)
{
	int	door_top;
	int	door_bottom;
	int	raw_top;

	if (!ray->hit_door || depth <= ray->door_distance)
		return (false);
	get_door_top_bottom(&door_top, &door_bottom, &raw_top, ray, g);
	return (y >= door_top && y < door_bottom);
}
