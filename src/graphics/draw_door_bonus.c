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

	true_distance = ray->door_distance / cos(g->player.orientation
			- ray->angle);
	pos.x = g->player.pos.x + true_distance * ray->dir.x;
	pos.y = g->player.pos.y + true_distance * ray->dir.y;
	pos.x -= ray->dir.x * 0.01;
	pos.y -= ray->dir.y * 0.01;
	return (pos);
}

static bool	is_secret_door(t_ray *ray, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.secret_count)
	{
		if (&g->map.doors[g->map.secrets[i].y]
			[g->map.secrets[i].x] == ray->hit_door)
			return (true);
		i++;
	}
	return (false);
}

static t_texture	*door_texture(t_ray *ray, t_game *g)
{
	int		texture;

	if (!is_secret_door(ray, g))
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
	t_dimensions	door;

	get_door_top_bottom(&door, ray, g);
	slice.screen_x = ray->x;
	slice.y_start = door.top;
	slice.y_end = door.bottom;
	slice.raw_top = door.raw_top;
	slice.texture = door_texture(ray, g);
	slice.texture_x = get_texture_x(ray, ray->door_distance, ray->door_side, g);
	slice.viewer_distance = ray->door_distance;
	slice.light = get_light_at(g, door_world_pos(ray, g));
	slice.ray = ray;
	draw_texture_slice(&slice, g);
}

void	get_door_top_bottom(t_dimensions *door, t_ray *ray, t_game *g)
{
	int			visible_door_height;
	int			full_top;
	int			full_bottom;
	t_position	pos;
	t_position	heights;

	pos = door_world_pos(ray, g);
	heights.x = get_floor_z_at(g, pos);
	heights.y = get_ceiling_z_at(g, pos);
	full_top = project_world_z(heights.y, ray->door_distance, g);
	full_bottom = project_world_z(heights.x, ray->door_distance, g);
	visible_door_height = (full_bottom - full_top) * (1.0
			- ray->hit_door->opening_state);
	door->bottom = full_bottom;
	door->top = door->bottom - visible_door_height;
	door->raw_top = full_top;
	if (door->top < 0)
		door->top = 0;
	if (door->bottom > WIN_HEIGHT)
		door->bottom = WIN_HEIGHT;
}
