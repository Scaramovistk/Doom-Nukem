/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	is_sprite_transparent(int transparent_color, int color)
{
	return ((color & 0x00FFFFFF) == 0x00FF00FF || color == transparent_color);
}

int	sprite_texture_size(t_texture *texture, t_game *g)
{
	int	i;

	i = 0;
	while (i < ITEM_TYPES_NB)
	{
		if (texture == &g->assets.item_icons[i])
			return (32);
		i++;
	}
	if (texture == &g->assets.item_icons[ITEM_ARTIFACT])
		return (32);
	return (TEXTURE_SIZE);
}

int	get_glass_pixel(t_transparent_hit *hit, int y, t_ray *ray,
		t_game *g)
{
	t_dimensions	wall;
	double			tex_y_pos;
	double			wall_height;
	int				tex_x;
	int				tex_y;

	ray->distance = hit->distance;
	ray->side = hit->side;
	get_wall_top_bottom(&wall, ray, g);
	if (y < wall.top || y >= wall.bottom)
		return (-1);
	wall_height = WIN_HEIGHT / hit->distance;
	tex_x = (int)(get_texture_x(ray, hit->distance, hit->side, g)
			* (double)TEXTURE_SIZE);
	tex_y_pos = (y - wall.raw_top) * (1.0 * TEXTURE_SIZE / wall_height);
	tex_y = ((int)tex_y_pos) & (TEXTURE_SIZE - 1);
	return (get_pixel(&g->assets.textures[TRANSPARENT_T].img, tex_x, tex_y));
}

int	blend_sprite_glass(int color, int y, t_sprite_glass *glass)
{
	int		i;
	int		glass_color;
	double	save_distance;
	int		save_side;

	if (!glass->g->assets.textures[TRANSPARENT_T].img.ptr
		|| !glass->ray->transparent_count)
		return (color);
	save_distance = glass->ray->distance;
	save_side = glass->ray->side;
	i = glass->ray->transparent_count;
	while (i--)
	{
		if (glass->sprite_depth <= glass->ray->transparent_hits[i].distance)
			continue ;
		glass_color = get_glass_pixel(&glass->ray->transparent_hits[i], y,
				glass->ray, glass->g);
		if (glass_color >= 0 && (glass_color & 0x00FFFFFF) != 0x00FF00FF)
			color = blend_color(color, glass_color, 0.45);
	}
	glass->ray->distance = save_distance;
	glass->ray->side = save_side;
	return (color);
}
