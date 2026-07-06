/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:48:54 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:49:07 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_int_image(t_img *img)
{
	img->ptr = NULL;
	img->addr = NULL;
	img->bits_per_pixel = 0;
	img->line_length = 0;
	img->endian = 0;
}

void	ft_int_assets(t_assets *assets)
{
	int	i;

	i = TEXTURES_NB;
	while (i--)
	{
		ft_int_image(&assets->textures[i].img);
		assets->textures[i].source = NULL;
	}
	ft_int_image(&assets->floor_texture.img);
	ft_int_image(&assets->ceiling_texture.img);
	ft_int_image(&assets->sky_texture.img);
	assets->floor_texture.source = NULL;
	assets->ceiling_texture.source = NULL;
	assets->sky_texture.source = NULL;
	i = SPRITE_FRAME_NB;
	while (i--)
	{
		ft_int_image(&assets->sprite_frames[i].img);
		assets->sprite_frames[i].source = NULL;
	}
	assets->has_sky = false;
	assets->has_sprite_frames = false;
	assets->ceiling_color = -1;
	assets->floor_color = -1;
}

void	ft_int_player(t_player *p)
{
	p->pos.x = 0;
	p->pos.y = 0;
	p->rotation_move = 0;
	p->key_rotation_move = 0;
	p->pitch = 0;
	p->pitch_move = 0;
	p->key_pitch_move = 0;
	p->z = PLAYER_FLOOR_Z;
	p->z_velocity = 0;
	p->eye_height = PLAYER_STAND_HEIGHT;
	p->mouse_move_pending = false;
	p->is_running = false;
	p->is_crouching = false;
	p->is_flying = false;
	p->is_swimming = false;
	p->on_ground = true;
	p->fly_move = 0;
	p->vertical_move = 0;
	p->lateral_move = 0;
	p->orientation = 0.0;
	p->look = ' ';
	p->mouse.x = WIN_WIDTH / 2;
	p->mouse.y = WIN_HEIGHT / 2;
}

void	ft_int_hud(t_hud *hud)
{
	int	i;

	hud->health = 100;
	hud->max_health = 100;
	hud->ammo = 30;
	hud->score = 0;
	i = 0;
	while (i < 4)
		hud->inventory[i++] = 0;
}

void	ft_init_map(t_map *map)
{
	map->grid = NULL;
	map->sprites = NULL;
	map->sprite_count = 0;
	map->items = NULL;
	map->item_count = 0;
	map->switches = NULL;
	map->switch_count = 0;
	map->hazard_zones = NULL;
	map->hazard_count = 0;
	map->message_zones = NULL;
	map->message_count = 0;
	map->width = 0;
	map->height = 0;
}

void	ft_int_message(t_message *message)
{
	message->text[0] = '\0';
	message->timer = 0.0;
}

void	init_game_struct(t_game *g)
{
	g->allocated_pointers = NULL;
	g->exit_status = EXIT_SUCCESS;
	ft_init_map(&g->map);
	ft_int_player(&g->player);
	ft_int_hud(&g->hud);
	ft_int_message(&g->message);
	ft_int_assets(&g->assets);
	ft_int_image(&g->img);
	g->mlx = NULL;
	g->mlx_win = NULL;
}
