/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_surface.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	scale_color(int color, double scale)
{
	int	red;
	int	green;
	int	blue;

	red = fmin(255, ((color >> 16) & 0xFF) * scale);
	green = fmin(255, ((color >> 8) & 0xFF) * scale);
	blue = fmin(255, (color & 0xFF) * scale);
	return ((red << 16) | (green << 8) | blue);
}

static void	fill_surface_texture(t_texture *t, int color, t_game *g)
{
	int	x;
	int	y;
	int	pixel;

	t->img.ptr = mlx_new_image(g->mlx, TEXTURE_SIZE, TEXTURE_SIZE);
	if (!t->img.ptr)
		error("Unable to create floor texture", g);
	t->img.addr = mlx_get_data_addr(t->img.ptr, &t->img.bits_per_pixel,
			&t->img.line_length, &t->img.endian);
	y = -1;
	while (++y < TEXTURE_SIZE)
	{
		x = -1;
		while (++x < TEXTURE_SIZE)
		{
			pixel = scale_color(color, 0.75 + 0.2 * (((x / 8) + (y / 8)) & 1));
			put_pixel(&t->img, x, y, pixel);
		}
	}
}

void	load_floor_ceiling_textures(t_game *g)
{
	int	size;

	size = TEXTURE_SIZE;
	if (g->assets.floor_texture.source)
		load_texture(&g->assets.floor_texture, &size, g);
	else
		fill_surface_texture(&g->assets.floor_texture,
			g->assets.floor_color, g);
	size = TEXTURE_SIZE;
	if (g->assets.ceiling_texture.source)
		load_texture(&g->assets.ceiling_texture, &size, g);
	else
		fill_surface_texture(&g->assets.ceiling_texture,
			g->assets.ceiling_color, g);
	size = TEXTURE_SIZE;
	if (g->assets.has_sky)
		load_texture(&g->assets.sky_texture, &size, g);
}
