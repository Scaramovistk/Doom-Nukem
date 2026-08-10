/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	surface_color(t_texture *texture, t_floor_cast *cast, t_game *g)
{
	t_position	sample;
	int			tex_x;
	int			tex_y;
	int			color;

	tex_x = (int)(TEXTURE_SIZE * (cast->floor_x
				- floor(cast->floor_x))) & (TEXTURE_SIZE - 1);
	tex_y = (int)(TEXTURE_SIZE * (cast->floor_y
				- floor(cast->floor_y))) & (TEXTURE_SIZE - 1);
	sample = (t_position){cast->floor_x, cast->floor_y};
	color = get_pixel(&texture->img, tex_x, tex_y);
	return (apply_light(color, get_light_at(g, sample), cast->row_distance));
}

static void	draw_surface_row(int y, int horizon, t_floor_cast cast, t_game *g)
{
	t_texture	*texture;
	int			x;

	texture = &g->assets.floor_texture;
	if (y < horizon)
		texture = &g->assets.ceiling_texture;
	x = 0;
	while (x < WIN_WIDTH)
	{
		put_pixel(&g->img, x, y, surface_color(texture, &cast, g));
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
	tex_y = ((int)(((double)y - g->player.pitch) * TEXTURE_SIZE / (WIN_HEIGHT
					/ 2))) & (TEXTURE_SIZE - 1);
	x = 0;
	while (x < WIN_WIDTH)
	{
		tex_x = ((x * TEXTURE_SIZE / WIN_WIDTH) + angle_offset)
			& (TEXTURE_SIZE - 1);
		put_pixel(&g->img, x, y, get_pixel(&g->assets.sky_texture.img, tex_x,
				tex_y));
		x++;
	}
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
