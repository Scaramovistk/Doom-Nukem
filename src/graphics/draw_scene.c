/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:43 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:46 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_scene(t_game *g)
{
	t_ray	rays[WIN_WIDTH];

	cast_all_rays(rays, g);
	draw_all_rays(rays, g);
}

void	draw_all_rays(t_ray *rays, t_game *g)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		rays[x].x = x;
		draw_one_ray(&rays[x], g);
		x++;
	}
}

void	draw_one_ray(t_ray *ray, t_game *g)
{
	t_dimensions	wall;

	get_wall_top_bottom(&wall, ray);
	draw_vertical_line(ray->x, (t_dimensions){0, wall.top},
		g->assets.ceiling_color, g);
	draw_wall_slice(wall, ray, g);
	draw_vertical_line(ray->x, (t_dimensions){wall.bottom, WIN_HEIGHT},
		g->assets.floor_color, g);
	if (ray->hit_door)
		draw_door_slice(ray, g);
}

void	draw_vertical_line(int x, t_dimensions dimensions, int color, t_game *g)
{
	int	y;

	y = dimensions.top;
	while (y < dimensions.bottom)
	{
		put_pixel(&g->img, x, y, color);
		y++;
	}
}
