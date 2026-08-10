/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_projection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:43 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:46 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	sort_sprites(t_sprite_draw *sprites, int count)
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

void	init_sprite_draw(t_sprite_draw *s, t_position pos, int index,
		t_game *g)
{
	t_position	dir;
	t_position	plane;
	double		inv_det;
	double		plane_len;

	s->pos = pos;
	s->sprite_index = index;
	s->distance = pow(g->player.pos.x - pos.x, 2) + pow(g->player.pos.y - pos.y,
			2);
	dir.x = cos(g->player.orientation);
	dir.y = sin(g->player.orientation);
	plane_len = tan(deg_to_rad(FOV) / 2.0);
	plane.x = -dir.y * plane_len;
	plane.y = dir.x * plane_len;
	inv_det = 1.0 / (plane.x * dir.y - dir.x * plane.y);
	s->transform_x = inv_det * (dir.y * (pos.x - g->player.pos.x) - dir.x
			* (pos.y - g->player.pos.y));
	s->transform_y = inv_det * (-plane.y * (pos.x - g->player.pos.x) + plane.x
			* (pos.y - g->player.pos.y));
	s->screen_x = (int)((WIN_WIDTH / 2) * (1 + s->transform_x
				/ s->transform_y));
}

static void	clamp_sprite_bounds(t_sprite_draw *s)
{
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

void	set_sprite_bounds(t_sprite_draw *s, t_game *g)
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
	clamp_sprite_bounds(s);
}
