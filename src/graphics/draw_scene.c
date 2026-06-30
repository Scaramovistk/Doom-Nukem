/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:43 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:46 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_scene(t_game *g)
{
	t_ray	rays[WIN_WIDTH];
	double	z_buffer[WIN_WIDTH];

	draw_floor_ceiling(g);
	cast_all_rays(rays, g);
	draw_all_rays(rays, g, z_buffer);
	draw_sprites(g, z_buffer);
}

static void	init_floor_cast(t_floor_cast *cast, t_game *g)
{
	double	dir_x;
	double	dir_y;
	double	plane_len;

	dir_x = cos(g->player.orientation);
	dir_y = sin(g->player.orientation);
	plane_len = tan(deg_to_rad(FOV) / 2.0);
	cast->ray_dir_x0 = dir_x + dir_y * plane_len;
	cast->ray_dir_y0 = dir_y - dir_x * plane_len;
	cast->ray_dir_x1 = dir_x - dir_y * plane_len;
	cast->ray_dir_y1 = dir_y + dir_x * plane_len;
}

static void	draw_surface_row(int y, int horizon, t_floor_cast cast, t_game *g)
{
	t_texture	*texture;
	int			x;
	int			tex_x;
	int			tex_y;

	texture = &g->assets.floor_texture;
	if (y < horizon)
		texture = &g->assets.ceiling_texture;
	x = 0;
	while (x < WIN_WIDTH)
	{
		tex_x = (int)(TEXTURE_SIZE * (cast.floor_x - floor(cast.floor_x)))
			& (TEXTURE_SIZE - 1);
		tex_y = (int)(TEXTURE_SIZE * (cast.floor_y - floor(cast.floor_y)))
			& (TEXTURE_SIZE - 1);
		put_pixel(&g->img, x, y, get_pixel(&texture->img, tex_x, tex_y));
		cast.floor_x += cast.step_x;
		cast.floor_y += cast.step_y;
		x++;
	}
}

static void	draw_sky_row(int y, t_game *g)
{
	int	x;
	int	tex_x;
	int	tex_y;
	int	angle_offset;

	angle_offset = (int)((g->player.orientation / (2 * M_PI)) * TEXTURE_SIZE);
	tex_y = ((int)(((double)y - g->player.pitch) * TEXTURE_SIZE
				/ (WIN_HEIGHT / 2))) & (TEXTURE_SIZE - 1);
	x = 0;
	while (x < WIN_WIDTH)
	{
		tex_x = ((x * TEXTURE_SIZE / WIN_WIDTH) + angle_offset)
			& (TEXTURE_SIZE - 1);
		put_pixel(&g->img, x, y,
			get_pixel(&g->assets.sky_texture.img, tex_x, tex_y));
		x++;
	}
}

static void	set_surface_row(int y, int horizon, t_floor_cast *cast, t_game *g)
{
	int	row_offset;

	row_offset = y - horizon;
	if (row_offset < 0)
		row_offset = -row_offset;
	cast->row_distance = (0.5 * WIN_HEIGHT) / row_offset;
	cast->step_x = cast->row_distance * (cast->ray_dir_x1
			- cast->ray_dir_x0) / WIN_WIDTH;
	cast->step_y = cast->row_distance * (cast->ray_dir_y1
			- cast->ray_dir_y0) / WIN_WIDTH;
	cast->floor_x = g->player.pos.x + cast->row_distance
		* cast->ray_dir_x0;
	cast->floor_y = g->player.pos.y + cast->row_distance
		* cast->ray_dir_y0;
}

void	draw_floor_ceiling(t_game *g)
{
	t_floor_cast	cast;
	int				y;
	int				horizon;

	init_floor_cast(&cast, g);
	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		if (g->assets.has_sky && y < horizon)
			draw_sky_row(y, g);
		else if (y != horizon)
		{
			set_surface_row(y, horizon, &cast, g);
			draw_surface_row(y, horizon, cast, g);
		}
		y++;
	}
}

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
