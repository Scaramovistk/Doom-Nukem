/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_sprite_pixel(t_sprite_draw *s, t_coord pixel,
		t_sprite_stripe *ctx)
{
	t_sprite_glass	glass;
	t_coord			texture_pos;
	int				texture_size;
	int				color;

	texture_size = sprite_texture_size(ctx->texture, ctx->g);
	texture_pos.x = (int)(256 * (pixel.x - (-s->width / 2 + s->screen_x))
			* texture_size / s->width) / 256;
	texture_pos.y = (pixel.y - s->raw_top) * texture_size / s->height;
	color = get_pixel(&ctx->texture->img, texture_pos.x, texture_pos.y);
	if (is_sprite_transparent(ctx->transparent_color, color))
		return ;
	glass = (t_sprite_glass){ctx->g, ctx->ray, s->transform_y};
	color = apply_light(color, ctx->light, s->transform_y);
	color = blend_sprite_glass(color, pixel.y, &glass);
	if (!door_occludes_pixel(ctx->ray, s->transform_y, pixel.y, ctx->g)
		&& !height_step_occludes_pixel(ctx->ray, s->transform_y, pixel.y,
			ctx->g))
		put_pixel(&ctx->g->img, pixel.x, pixel.y, color);
}

void	adjust_laptop_sprite(t_sprite_draw *s, t_game *g, t_texture *texture)
{
	if (texture != &g->assets.laptop)
		return ;
	s->bottom += s->height * 11 / TEXTURE_SIZE;
	s->raw_top += s->height * 11 / TEXTURE_SIZE;
	s->top += s->height * 11 / TEXTURE_SIZE;
	if (s->bottom >= WIN_HEIGHT)
		s->bottom = WIN_HEIGHT - 1;
}

bool	sprite_assets_available(t_game *g)
{
	return (g->assets.textures[SPRITE_T].img.ptr
		|| g->assets.has_sprite_frames || g->assets.item_icons[0].img.ptr
		|| g->assets.enemy_icons[0].img.ptr
		|| g->assets.decoration_icons[ELEVATOR_BUTTON_DECORATION].img.ptr);
}

int	collect_visible_sprites(t_game *g, t_sprite_draw *sprites)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < g->map.sprite_count)
	{
		if (!(g->map.has_flag && g->map.flag_carried
				&& g->map.flag_sprite_index == i)
			&& bsp_position_visible(g, g->map.sprites[i]))
			init_sprite_draw(&sprites[count++], g->map.sprites[i], i, g);
		i++;
	}
	return (count);
}

bool	projectile_pixel_visible(t_coord pixel, t_projectile_draw *ctx)
{
	int	radius;

	radius = ctx->projectile->size;
	return (pixel.x >= 0 && pixel.x < WIN_WIDTH && pixel.y >= 0
		&& pixel.y < WIN_HEIGHT && pow(pixel.x - ctx->sprite->screen_x, 2)
		+ pow(pixel.y - (ctx->sprite->top + radius), 2) <= radius * radius
		&& !door_occludes_pixel(&ctx->rays[pixel.x], ctx->sprite->transform_y,
			pixel.y, ctx->g)
		&& !height_step_occludes_pixel(&ctx->rays[pixel.x],
			ctx->sprite->transform_y, pixel.y, ctx->g));
}
