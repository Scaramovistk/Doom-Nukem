/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_sprite_stripe(t_sprite_draw *s, int stripe, t_sprite_stripe *ctx)
{
	int	y;

	y = s->top;
	while (y < s->bottom)
	{
		draw_sprite_pixel(s, (t_coord){stripe, y}, ctx);
		y++;
	}
}

void	draw_one_sprite(t_sprite_draw *s, t_game *g, double *z_buffer,
		t_ray *rays)
{
	t_sprite_stripe	ctx;
	int				stripe;

	if (s->transform_y <= 0)
		return ;
	set_sprite_bounds(s, g);
	ctx.g = g;
	ctx.texture = get_sprite_texture(s, g);
	ctx.light = get_light_at(g, s->pos);
	ctx.transparent_color = get_pixel(&ctx.texture->img, 0, 0);
	adjust_laptop_sprite(s, g, ctx.texture);
	stripe = s->left;
	while (stripe < s->right)
	{
		if (s->transform_y > 0 && stripe > 0 && stripe < WIN_WIDTH
			&& s->transform_y < z_buffer[stripe])
		{
			ctx.ray = &rays[stripe];
			draw_sprite_stripe(s, stripe, &ctx);
		}
		stripe++;
	}
}

void	draw_sprites(t_game *g, double *z_buffer, t_ray *rays)
{
	t_sprite_draw	*sprites;
	int				i;
	int				count;

	if (!g->map.sprite_count)
		return ;
	if (!sprite_assets_available(g))
		return ;
	sprites = malloc(g->map.sprite_count * sizeof(t_sprite_draw));
	if (!sprites)
		return ;
	count = collect_visible_sprites(g, sprites);
	sort_sprites(sprites, count);
	i = 0;
	while (i < count)
	{
		if (sprites[i].distance >= 0.0)
			draw_one_sprite(&sprites[i], g, z_buffer, rays);
		i++;
	}
	free(sprites);
}

void	draw_projectile_dot(t_game *g, t_sprite_draw *s, t_projectile *p,
		t_ray *rays)
{
	t_projectile_draw	ctx;
	t_coord				pixel;
	int					radius;

	radius = p->size;
	ctx = (t_projectile_draw){g, s, p, rays};
	pixel.x = s->screen_x - radius;
	while (pixel.x <= s->screen_x + radius)
	{
		pixel.y = s->top;
		while (pixel.y <= s->bottom)
		{
			if (projectile_pixel_visible(pixel, &ctx))
				put_pixel(&g->img, pixel.x, pixel.y, p->color);
			pixel.y++;
		}
		pixel.x++;
	}
}

void	init_projectile_draw(t_sprite_draw *s, t_projectile *p, t_game *g)
{
	init_sprite_draw(s, p->pos, -1, g);
	s->height = p->size * 2;
	s->width = p->size * 2;
	s->top = (WIN_HEIGHT / 2) + (int)g->player.pitch - p->size;
	s->bottom = s->top + s->height;
}
