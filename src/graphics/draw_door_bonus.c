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

static double	get_eye_z(t_player *p)
{
	return (p->z + p->eye_height);
}

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

static int	project_world_z(double world_z, double distance, t_game *g)
{
	int		horizon;
	double	eye_z;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	eye_z = get_eye_z(&g->player);
	return (horizon - (int)((world_z - eye_z) * WIN_HEIGHT / distance));
}

void	draw_door_slice(t_ray *ray, t_game *g)
{
	t_texture_slice	slice;
	int				door_top;
	int				door_bottom;

	get_door_top_bottom(&door_top, &door_bottom, ray, g);
	slice.screen_x = ray->x;
	slice.y_start = door_top;
	slice.y_end = door_bottom;
	slice.texture = &g->assets.textures[DOOR_T];
	slice.texture_x = get_texture_x(ray, ray->door_distance, ray->door_side, g);
	slice.viewer_distance = ray->door_distance;
	slice.light = get_light_at(g, door_world_pos(ray, g));
	draw_texture_slice(&slice, g);
}

void	get_door_top_bottom(int *door_top, int *door_bottom,
		t_ray *ray, t_game *g)
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
	if (*door_top < 0)
		*door_top = 0;
	if (*door_bottom > WIN_HEIGHT)
		*door_bottom = WIN_HEIGHT;
}
