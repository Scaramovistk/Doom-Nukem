/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:52 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:54 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

void	get_wall_top_bottom(t_dimensions *wall, t_ray *ray)
{
	int	wall_height;

	wall_height = (int)(WIN_HEIGHT / ray->distance);
	wall->top = (WIN_HEIGHT / 2) - (wall_height / 2);
	wall->bottom = (WIN_HEIGHT / 2) + (wall_height / 2);
	if (wall->top < 0)
		wall->top = 0;
	if (wall->bottom > WIN_HEIGHT)
		wall->bottom = WIN_HEIGHT;
}
