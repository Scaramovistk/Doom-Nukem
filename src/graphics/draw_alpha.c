/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_alpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	alpha_pixel_visible(t_texture_slice *s, t_game *g, int color,
		int y)
{
	if ((color & 0x00FFFFFF) == 0x00FF00FF)
		return (false);
	if (s->texture->has_transparent_color && (color & 0x00FFFFFF)
		== (s->texture->transparent_color & 0x00FFFFFF))
		return (false);
	if (s->ray && door_occludes_pixel(s->ray, s->viewer_distance, y, g))
		return (false);
	if (s->ray && height_step_occludes_pixel(s->ray,
			s->viewer_distance, y, g))
		return (false);
	return (true);
}

void	draw_texture_slice_alpha(t_texture_slice *s, t_game *g)
{
	double	texture_y_pos;
	int		texture_y;
	int		color;
	int		y;

	s->texture_x_size = (int)(s->texture_x * (double)TEXTURE_SIZE);
	s->height = WIN_HEIGHT / s->viewer_distance;
	s->texture_step = 1.0 * TEXTURE_SIZE / s->height;
	texture_y_pos = (s->y_start - s->raw_top) * s->texture_step;
	y = s->y_start;
	while (y < s->y_end)
	{
		texture_y = ((int)texture_y_pos) & (TEXTURE_SIZE - 1);
		texture_y_pos += s->texture_step;
		color = get_pixel(&s->texture->img, s->texture_x_size, texture_y);
		if (alpha_pixel_visible(s, g, color, y))
		{
			color = apply_light(color, s->light, s->viewer_distance);
			put_pixel(&g->img, s->screen_x, y, blend_color(get_pixel(&g->img,
						s->screen_x, y), color, 0.45));
		}
		y++;
	}
}
