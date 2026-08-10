/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	ft_int_player(t_player *p)
{
	ft_bzero(p, sizeof(*p));
	p->z = PLAYER_FLOOR_Z;
	p->eye_height = PLAYER_STAND_HEIGHT;
	p->on_ground = true;
	p->look = ' ';
	p->mouse.x = WIN_WIDTH / 2;
	p->mouse.y = WIN_HEIGHT / 2;
}

void	ft_int_hud(t_hud *hud)
{
	int	i;

	hud->health = 100;
	hud->max_health = 100;
	hud->magazine[0] = PISTOL_MAGAZINE_SIZE;
	hud->magazine[1] = BLASTER_MAGAZINE_SIZE;
	hud->score = 0;
	hud->fps = 0;
	hud->selected_item = 0;
	hud->selected_weapon = 0;
	hud->weapon_flash = 0.0;
	i = 0;
	while (i < 4)
		hud->inventory[i++] = 0;
	hud->inventory[ITEM_AMMO] = STARTING_RESERVE_AMMO;
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
	level->next_level[0] = '\0';
}
