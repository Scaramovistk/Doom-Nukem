/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:40 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:42 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_door_slice(t_ray *ray, t_game *g)
{
	t_texture_slice	slice;
	int				door_top;
	int				door_bottom;

	get_door_top_bottom(&door_top, &door_bottom, ray);
	slice.screen_x = ray->x;
	slice.y_start = door_top;
	slice.y_end = door_bottom;
	slice.texture = &g->assets.textures[DOOR_T];
	slice.texture_x = get_texture_x(ray, ray->door_distance, ray->door_side, g);
	slice.viewer_distance = ray->door_distance;
	draw_texture_slice(&slice, g);
}

void	get_door_top_bottom(int *door_top, int *door_bottom, t_ray *ray)
{
	int	full_door_height;
	int	visible_door_height;

	full_door_height = (int)(WIN_HEIGHT / ray->door_distance);
	visible_door_height = full_door_height * (1.0
			- ray->hit_door->opening_state);
	*door_bottom = (WIN_HEIGHT / 2) + (full_door_height / 2);
	*door_top = *door_bottom - visible_door_height;
	if (*door_top < 0)
		*door_top = 0;
	if (*door_bottom > WIN_HEIGHT)
		*door_bottom = WIN_HEIGHT;
}
