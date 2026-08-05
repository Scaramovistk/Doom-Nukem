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

static void	init_sprite_draw(t_sprite_draw *s, t_position pos, int index,
		t_game *g)
{
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	inv_det;
	double	plane_len;

	s->pos = pos;
	s->sprite_index = index;
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

static t_decoration	*sprite_decoration(t_sprite_draw *s, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == s->sprite_index)
			return (&g->map.decorations[i]);
		i++;
	}
	return (NULL);
}

static t_world_object	*sprite_object(t_sprite_draw *s, t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map.object_count)
	{
		if (g->map.objects[i].sprite_index == s->sprite_index)
			return (&g->map.objects[i]);
		i++;
	}
	return (NULL);
}

static void	set_sprite_bounds(t_sprite_draw *s, t_game *g)
{
	t_decoration	*decoration;
	t_world_object	*object;
	double			scale;
	double			bottom_z;

	scale = 1.0;
	bottom_z = get_floor_z_at(g, s->pos);
	decoration = sprite_decoration(s, g);
	if (decoration)
	{
		scale = decoration->scale;
		bottom_z += decoration->z_offset;
	}
	object = sprite_object(s, g);
	if (object)
		scale = object->scale;
	s->height = abs((int)(WIN_HEIGHT * scale / s->transform_y));
	s->width = s->height;
	s->bottom = project_world_z(bottom_z, s->transform_y, g);
	s->raw_top = s->bottom - s->height;
	s->top = s->raw_top;
	if (s->top < 0)
		s->top = 0;
	if (s->bottom >= WIN_HEIGHT)
		s->bottom = WIN_HEIGHT - 1;
	s->left = s->screen_x - s->width / 2;
	if (s->left < 0)
		s->left = 0;
	s->right = s->screen_x + s->width / 2;
	if (s->right >= WIN_WIDTH)
		s->right = WIN_WIDTH - 1;
}

static bool	is_sprite_transparent(t_texture *texture, int color)
{
	return ((color & 0x00FFFFFF) == 0x00FF00FF
		|| color == get_pixel(&texture->img, 0, 0));
}

static t_texture	*get_sprite_texture(t_sprite_draw *s, t_game *g)
{
	double	angle;
	int		frame;
	int		i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active
			&& g->map.items[i].sprite_index == s->sprite_index
			&& g->map.items[i].type >= 0
			&& g->map.items[i].type < ITEM_TYPES_NB)
			return (&g->assets.item_icons[g->map.items[i].type]);
		i++;
	}
	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == s->sprite_index
			&& g->assets.decoration_icons[g->map.decorations[i].type].img.ptr)
			return (&g->assets.decoration_icons[g->map.decorations[i].type]);
		i++;
	}
	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active
			&& g->map.enemies[i].sprite_index == s->sprite_index
			&& g->map.enemies[i].type >= 0
			&& g->map.enemies[i].type < ENEMY_TYPES_NB
			&& g->assets.enemy_icons[g->map.enemies[i].type].img.ptr)
			return (&g->assets.enemy_icons[g->map.enemies[i].type]);
		i++;
	}
	if (!g->assets.has_sprite_frames)
		return (&g->assets.textures[SPRITE_T]);
	angle = normalize_angle(atan2(g->player.pos.y - s->pos.y,
				g->player.pos.x - s->pos.x));
	frame = (int)((angle + (M_PI / SPRITE_FRAME_NB))
			/ (2 * M_PI / SPRITE_FRAME_NB)) % SPRITE_FRAME_NB;
	return (&g->assets.sprite_frames[frame]);
}

static int	sprite_texture_size(t_texture *texture, t_game *g)
{
	int	i;

	i = 0;
	while (i < ITEM_TYPES_NB)
	{
		if (texture == &g->assets.item_icons[i])
			return (32);
		i++;
	}
	return (TEXTURE_SIZE);
}

static int	get_glass_pixel(t_transparent_hit *hit, int y, t_ray *ray, t_game *g)
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

static int	blend_sprite_glass(int color, int y, double sprite_depth,
		t_ray *ray, t_game *g)
{
	int		i;
	int		glass_color;
	double	save_distance;
	int		save_side;

	if (!g->assets.textures[TRANSPARENT_T].img.ptr)
		return (color);
	save_distance = ray->distance;
	save_side = ray->side;
	i = ray->transparent_count;
	while (i--)
	{
		if (sprite_depth <= ray->transparent_hits[i].distance)
			continue ;
		glass_color = get_glass_pixel(&ray->transparent_hits[i], y, ray, g);
		if (glass_color >= 0 && (glass_color & 0x00FFFFFF) != 0x00FF00FF)
			color = blend_color(color, glass_color, 0.45);
	}
	ray->distance = save_distance;
	ray->side = save_side;
	return (color);
}

static void	draw_sprite_stripe(t_sprite_draw *s, int stripe, t_game *g,
		t_texture *texture, t_ray *ray)
{
	int	tex_x;
	int	tex_y;
	int	y;
	int	color;
	int	texture_size;

	texture_size = sprite_texture_size(texture, g);
	tex_x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
			* texture_size / s->width) / 256;
	y = s->top;
	while (y < s->bottom)
	{
		tex_y = (y - s->raw_top) * texture_size / s->height;
		color = get_pixel(&texture->img, tex_x, tex_y);
		if (!is_sprite_transparent(texture, color))
		{
			color = apply_light(color, get_light_at(g, s->pos), s->transform_y);
			color = blend_sprite_glass(color, y, s->transform_y, ray, g);
			if (!door_occludes_pixel(ray, s->transform_y, y, g)
				&& !height_step_occludes_pixel(ray, s->transform_y, y, g))
				put_pixel(&g->img, stripe, y, color);
		}
		y++;
	}
}

static void	draw_one_sprite(t_sprite_draw *s, t_game *g, double *z_buffer,
		t_ray *rays)
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
			draw_sprite_stripe(s, stripe, g, texture, &rays[stripe]);
		stripe++;
	}
}

void	draw_sprites(t_game *g, double *z_buffer, t_ray *rays)
{
	t_sprite_draw	*sprites;
	int				i;

	if (!g->map.sprite_count)
		return ;
	if (!g->assets.textures[SPRITE_T].img.ptr && !g->assets.has_sprite_frames
		&& !g->assets.item_icons[0].img.ptr
		&& !g->assets.enemy_icons[0].img.ptr
		&& !g->assets.decoration_icons[ELEVATOR_BUTTON_DECORATION].img.ptr)
		return ;
	sprites = malloc(g->map.sprite_count * sizeof(t_sprite_draw));
	if (!sprites)
		return ;
	i = 0;
	while (i < g->map.sprite_count)
	{
		init_sprite_draw(&sprites[i], g->map.sprites[i], i, g);
		i++;
	}
	sort_sprites(sprites, g->map.sprite_count);
	i = 0;
	while (i < g->map.sprite_count)
		draw_one_sprite(&sprites[i++], g, z_buffer, rays);
	free(sprites);
}

static void	draw_projectile_dot(t_game *g, t_sprite_draw *s, t_projectile *p,
		t_ray *rays)
{
	int	x;
	int	y;
	int	radius;

	radius = p->size;
	x = s->screen_x - radius;
	while (x <= s->screen_x + radius)
	{
		y = s->top;
		while (y <= s->bottom)
		{
			if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT
				&& pow(x - s->screen_x, 2) + pow(y - (s->top + radius), 2)
				<= radius * radius && !door_occludes_pixel(&rays[x],
					s->transform_y, y, g)
				&& !height_step_occludes_pixel(&rays[x], s->transform_y, y, g))
				put_pixel(&g->img, x, y, p->color);
			y++;
		}
		x++;
	}
}

static void	init_projectile_draw(t_sprite_draw *s, t_projectile *p, t_game *g)
{
	init_sprite_draw(s, p->pos, -1, g);
	s->height = p->size * 2;
	s->width = p->size * 2;
	s->top = (WIN_HEIGHT / 2) + (int)g->player.pitch - p->size;
	s->bottom = s->top + s->height;
}

void	draw_projectiles(t_game *g, double *z_buffer, t_ray *rays)
{
	t_sprite_draw	s;
	int				i;

	i = 0;
	while (i < PROJECTILE_MAX)
	{
		if (g->projectiles[i].active)
		{
			init_projectile_draw(&s, &g->projectiles[i], g);
			if (s.transform_y > 0 && s.screen_x >= 0 && s.screen_x < WIN_WIDTH
				&& s.transform_y < z_buffer[s.screen_x])
				draw_projectile_dot(g, &s, &g->projectiles[i], rays);
		}
		i++;
	}
}
