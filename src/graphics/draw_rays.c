/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_all_rays(t_ray *rays, t_game *g, double *z_buffer)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		rays[x].x = x;
		draw_one_ray(&rays[x], g);
		z_buffer[x] = rays[x].distance;
		x++;
	}
}

void	draw_one_ray(t_ray *ray, t_game *g)
{
	t_dimensions	wall;

	draw_height_steps(ray, g);
	get_wall_top_bottom(&wall, ray, g);
	draw_wall_slice(wall, ray, g);
	if (ray->hit_block == DECAL_WALL)
		draw_wall_decal(wall, ray, g);
	if (ray->hit_door)
		draw_door_slice(ray, g);
	draw_transparent_walls(ray, g);
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
