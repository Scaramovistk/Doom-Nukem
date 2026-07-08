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
	int	j;

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
	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
		{
			ft_int_image(&assets->hud_weapons[i][j].img);
			assets->hud_weapons[i][j].source = NULL;
			j++;
		}
		i++;
	}
	assets->hud_weapons[0][0].source = HUD_PISTOL_IDLE;
	assets->hud_weapons[0][1].source = HUD_PISTOL_FIRE;
	assets->hud_weapons[1][0].source = HUD_BLASTER_IDLE;
	assets->hud_weapons[1][1].source = HUD_BLASTER_FIRE;
	ft_int_image(&assets->ammo_icon.img);
	assets->ammo_icon.source = HUD_AMMO_ICON;
	i = 0;
	while (i < ITEM_TYPES_NB)
	{
		ft_int_image(&assets->item_icons[i].img);
		assets->item_icons[i].source = NULL;
		i++;
	}
	assets->item_icons[0].source = HUD_ITEM0_ICON;
	assets->item_icons[1].source = HUD_ITEM1_ICON;
	assets->item_icons[2].source = HUD_ITEM2_ICON;
	assets->item_icons[3].source = HUD_ITEM3_ICON;
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
	hud->fps = 0;
	hud->selected_item = 0;
	hud->selected_weapon = 0;
	hud->weapon_flash = 0.0;
	i = 0;
	while (i < 4)
		hud->inventory[i++] = 0;
}

void	ft_init_map(t_map *map)
{
	map->grid = NULL;
	map->sprites = NULL;
	map->sprite_count = 0;
	map->enemies = NULL;
	map->enemy_count = 0;
	map->items = NULL;
	map->item_count = 0;
	map->switches = NULL;
	map->switch_count = 0;
	map->hazard_zones = NULL;
	map->hazard_count = 0;
	map->message_zones = NULL;
	map->message_count = 0;
	map->exit_zones = NULL;
	map->exit_count = 0;
	map->width = 0;
	map->height = 0;
}

void	ft_int_message(t_message *message)
{
	message->text[0] = '\0';
	message->timer = 0.0;
}

void	ft_int_level_flow(t_level_flow *level)
{
	level->started = false;
	level->completed = false;
	level->failed = false;
	level->end_timer = 0.0;
	level->required_items = 0;
}

void	ft_int_audio(t_audio *audio)
{
	audio->enabled = true;
	audio->music_pid = -1;
	audio->music_path[0] = '\0';
}

void	ft_int_projectiles(t_projectile *projectiles)
{
	int	i;

	i = 0;
	while (i < PROJECTILE_MAX)
	{
		projectiles[i].pos = (t_position){0.0, 0.0};
		projectiles[i].velocity = (t_position){0.0, 0.0};
		projectiles[i].ttl = 0.0;
		projectiles[i].damage = 0;
		projectiles[i].size = PROJECTILE_SIZE;
		projectiles[i].color = YELLOW;
		projectiles[i].active = false;
		i++;
	}
}

static void	ft_int_menu(t_menu *menu)
{
	int	i;

	menu->level_count = 0;
	menu->selected = 0;
	menu->difficulty = 1;
	menu->active = false;
	i = 0;
	while (i < MENU_MAX_LEVELS)
		menu->levels[i++][0] = '\0';
}

void	ft_int_events(t_world_event *events)
{
	int	i;

	i = 0;
	while (i < WORLD_EVENT_MAX)
	{
		events[i].action = EVENT_NONE;
		events[i].timer = 0.0;
		events[i].reload = 0.0;
		events[i].value = 0;
		events[i].repeat = false;
		events[i].active = false;
		events[i].message[0] = '\0';
		i++;
	}
}

void	init_game_struct(t_game *g)
{
	g->allocated_pointers = NULL;
	g->exit_status = EXIT_SUCCESS;
	g->state = STATE_PLAYING;
	ft_int_menu(&g->menu);
	ft_init_map(&g->map);
	ft_int_player(&g->player);
	ft_int_hud(&g->hud);
	ft_int_message(&g->message);
	ft_int_level_flow(&g->level);
	ft_int_audio(&g->audio);
	ft_int_projectiles(g->projectiles);
	ft_int_events(g->events);
	ft_int_assets(&g->assets);
	ft_int_image(&g->img);
	g->mlx = NULL;
	g->mlx_win = NULL;
}
