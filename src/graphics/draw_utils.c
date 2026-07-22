/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:47 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:50 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

double	get_texture_x(t_ray *ray, double distance, int side, t_game *g)
{
	double	tex_x;

	if (ray->hit_segment && distance == ray->distance)
		return (ray->segment_u);
	distance /= cos(g->player.orientation - ray->angle);
	if (side == 0)
		tex_x = g->player.pos.y + distance * ray->dir.y;
	else
		tex_x = g->player.pos.x + distance * ray->dir.x;
	tex_x -= floor(tex_x);
	if ((side == 0 && ray->dir.x > 0) || (side == 1 && ray->dir.y < 0))
		tex_x = 1.0 - tex_x;
	return (tex_x);
}

void	draw_texture_slice(t_texture_slice *s, t_game *g)
{
	double	texture_y_pos;
	int		texture_y;
	int		pixel_color;
	int		screen_y;
	int		horizon;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	s->texture_x_size = (int)(s->texture_x * (double)TEXTURE_SIZE);
	s->height = WIN_HEIGHT / s->viewer_distance;
	s->texture_step = 1.0 * TEXTURE_SIZE / s->height;
	texture_y_pos = (s->y_start - horizon + s->height / 2)
		* s->texture_step;
	screen_y = s->y_start;
	while (screen_y < s->y_end)
	{
		texture_y = ((int)(texture_y_pos)) & (TEXTURE_SIZE - 1);
		texture_y_pos += s->texture_step;
		pixel_color = get_pixel(&s->texture->img, s->texture_x_size, texture_y);
		pixel_color = apply_light(pixel_color, s->light, s->viewer_distance);
		put_pixel(&g->img, s->screen_x, screen_y, pixel_color);
		screen_y++;
	}
}

int	blend_color(int dst, int src, double alpha)
{
	int	red;
	int	green;
	int	blue;

	red = ((src >> 16) & 0xFF) * alpha + ((dst >> 16) & 0xFF)
		* (1.0 - alpha);
	green = ((src >> 8) & 0xFF) * alpha + ((dst >> 8) & 0xFF)
		* (1.0 - alpha);
	blue = (src & 0xFF) * alpha + (dst & 0xFF) * (1.0 - alpha);
	return ((red << 16) | (green << 8) | blue);
}

void	draw_texture_slice_alpha(t_texture_slice *s, t_game *g)
{
	double	texture_y_pos;
	int		texture_y;
	int		pixel_color;
	int		screen_y;
	int		horizon;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	s->texture_x_size = (int)(s->texture_x * (double)TEXTURE_SIZE);
	s->height = WIN_HEIGHT / s->viewer_distance;
	s->texture_step = 1.0 * TEXTURE_SIZE / s->height;
	texture_y_pos = (s->y_start - horizon + s->height / 2)
		* s->texture_step;
	screen_y = s->y_start;
	while (screen_y < s->y_end)
	{
		texture_y = ((int)(texture_y_pos)) & (TEXTURE_SIZE - 1);
		texture_y_pos += s->texture_step;
		pixel_color = get_pixel(&s->texture->img, s->texture_x_size, texture_y);
		if ((pixel_color & 0x00FFFFFF) != 0x00FF00FF)
		{
			pixel_color = apply_light(pixel_color, s->light,
					s->viewer_distance);
			put_pixel(&g->img, s->screen_x, screen_y,
				blend_color(get_pixel(&g->img, s->screen_x, screen_y),
					pixel_color, 0.45));
		}
		screen_y++;
	}
}

int	get_pixel(t_img *img, int x, int y)
{
	int	y_cord;
	int	bits;

	bits = img->bits_per_pixel / 8;
	y_cord = y * img->line_length;
	return (*(int *)(img->addr + y_cord + x * bits));
}
