/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:43 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:46 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	sort_sprites(t_sprite_draw *sprites, int count)
{
	t_sprite_draw	tmp;
	int				i;
	int				j;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (sprites[i].distance < sprites[j].distance)
			{
				tmp = sprites[i];
				sprites[i] = sprites[j];
				sprites[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	init_sprite_draw(t_sprite_draw *s, t_position pos, t_game *g)
{
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	inv_det;
	double	plane_len;

	s->pos = pos;
	s->distance = pow(g->player.pos.x - pos.x, 2)
		+ pow(g->player.pos.y - pos.y, 2);
	dir_x = cos(g->player.orientation);
	dir_y = sin(g->player.orientation);
	plane_len = tan(deg_to_rad(FOV) / 2.0);
	plane_x = -dir_y * plane_len;
	plane_y = dir_x * plane_len;
	inv_det = 1.0 / (plane_x * dir_y - dir_x * plane_y);
	s->transform_x = inv_det * (dir_y * (pos.x - g->player.pos.x)
			- dir_x * (pos.y - g->player.pos.y));
	s->transform_y = inv_det * (-plane_y * (pos.x - g->player.pos.x)
			+ plane_x * (pos.y - g->player.pos.y));
	s->screen_x = (int)((WIN_WIDTH / 2) * (1 + s->transform_x
				/ s->transform_y));
}

static void	set_sprite_bounds(t_sprite_draw *s, t_game *g)
{
	int	horizon;

	horizon = (WIN_HEIGHT / 2) + (int)g->player.pitch;
	s->height = abs((int)(WIN_HEIGHT / s->transform_y));
	s->width = s->height;
	s->top = horizon - s->height / 2;
	if (s->top < 0)
		s->top = 0;
	s->bottom = horizon + s->height / 2;
	if (s->bottom >= WIN_HEIGHT)
		s->bottom = WIN_HEIGHT - 1;
	s->left = s->screen_x - s->width / 2;
	if (s->left < 0)
		s->left = 0;
	s->right = s->screen_x + s->width / 2;
	if (s->right >= WIN_WIDTH)
		s->right = WIN_WIDTH - 1;
}

static t_texture	*get_sprite_texture(t_sprite_draw *s, t_game *g)
{
	double	angle;
	int		frame;

	if (!g->assets.has_sprite_frames)
		return (&g->assets.textures[SPRITE_T]);
	angle = normalize_angle(atan2(g->player.pos.y - s->pos.y,
				g->player.pos.x - s->pos.x));
	frame = (int)((angle + (M_PI / SPRITE_FRAME_NB))
			/ (2 * M_PI / SPRITE_FRAME_NB)) % SPRITE_FRAME_NB;
	return (&g->assets.sprite_frames[frame]);
}

static void	draw_sprite_stripe(t_sprite_draw *s, int stripe, t_game *g,
		t_texture *texture)
{
	int	tex_x;
	int	tex_y;
	int	y;
	int	color;
	int	d;

	tex_x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
			* TEXTURE_SIZE / s->width) / 256;
	y = s->top;
	while (y < s->bottom)
	{
		d = (y - ((WIN_HEIGHT / 2) + (int)g->player.pitch)) * 256
			+ s->height * 128;
		tex_y = ((d * TEXTURE_SIZE) / s->height) / 256;
		color = get_pixel(&texture->img, tex_x, tex_y);
		if ((color & 0x00FFFFFF) != 0x00FF00FF)
			put_pixel(&g->img, stripe, y, color);
		y++;
	}
}

static void	draw_one_sprite(t_sprite_draw *s, t_game *g, double *z_buffer)
{
	int	stripe;
	t_texture	*texture;

	if (s->transform_y <= 0)
		return ;
	set_sprite_bounds(s, g);
	texture = get_sprite_texture(s, g);
	stripe = s->left;
	while (stripe < s->right)
	{
		if (s->transform_y > 0 && stripe > 0 && stripe < WIN_WIDTH
			&& s->transform_y < z_buffer[stripe])
			draw_sprite_stripe(s, stripe, g, texture);
		stripe++;
	}
}

void	draw_sprites(t_game *g, double *z_buffer)
{
	t_sprite_draw	*sprites;
	int				i;

	if (!g->map.sprite_count || (!g->assets.textures[SPRITE_T].img.ptr
			&& !g->assets.has_sprite_frames))
		return ;
	sprites = malloc(g->map.sprite_count * sizeof(t_sprite_draw));
	if (!sprites)
		return ;
	i = 0;
	while (i < g->map.sprite_count)
	{
		init_sprite_draw(&sprites[i], g->map.sprites[i], g);
		i++;
	}
	sort_sprites(sprites, g->map.sprite_count);
	i = 0;
	while (i < g->map.sprite_count)
		draw_one_sprite(&sprites[i++], g, z_buffer);
	free(sprites);
}
