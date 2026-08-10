/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_band_render.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	band_row_bound(double camera_height, double distance)
{
	double	offset;

	camera_height = fabs(camera_height);
	if (camera_height < 0.02)
		camera_height = 0.02;
	if (distance < 0.05)
		distance = 0.05;
	offset = camera_height * WIN_HEIGHT / distance;
	if (offset > WIN_HEIGHT)
		offset = WIN_HEIGHT;
	return ((int)offset);
}

static int	sample_texture(t_texture *tex, t_position world)
{
	int	tex_x;
	int	tex_y;

	tex_x = (int)(TEXTURE_SIZE * (world.x - floor(world.x))) & (TEXTURE_SIZE
			- 1);
	tex_y = (int)(TEXTURE_SIZE * (world.y - floor(world.y))) & (TEXTURE_SIZE
			- 1);
	return (get_pixel(&tex->img, tex_x, tex_y));
}

static void	draw_band_pixel(t_step_ctx *ctx, int y, int horizon, t_game *g)
{
	double		row_distance;
	double		true_distance;
	t_position	world;
	int			color;

	row_distance = fabs(ctx->camera_height) * WIN_HEIGHT / abs(y - horizon);
	true_distance = row_distance * ctx->inv_cos;
	world.x = g->player.pos.x + true_distance * ctx->ray->dir.x;
	world.y = g->player.pos.y + true_distance * ctx->ray->dir.y;
	if (ctx->is_floor)
		color = sample_texture(&g->assets.floor_texture, world);
	else
		color = sample_texture(&g->assets.ceiling_texture, world);
	color = apply_light(color, get_light_at(g, world), row_distance);
	put_pixel(&g->img, ctx->ray->x, y, color);
}

void	draw_step_band(t_step_ctx ctx, t_game *g)
{
	int	horizon;
	int	y;
	int	y_end;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	if (ctx.camera_height <= 0)
	{
		y = horizon + band_row_bound(ctx.camera_height, ctx.far_d);
		y_end = horizon + band_row_bound(ctx.camera_height, ctx.near_d);
	}
	else
	{
		y = horizon - band_row_bound(ctx.camera_height, ctx.near_d);
		y_end = horizon - band_row_bound(ctx.camera_height, ctx.far_d);
	}
	if (y < 0)
		y = 0;
	if (y_end >= WIN_HEIGHT)
		y_end = WIN_HEIGHT - 1;
	while (y <= y_end)
		draw_band_pixel(&ctx, y++, horizon, g);
}
