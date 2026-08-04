/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_effects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/24 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	apply_health_pickup(t_game *g, int amount)
{
	(void)amount;
	g->hud.health = g->hud.max_health;
	show_message(g, "HEALTH PACK PICKED UP", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}

void	apply_ammo_pickup(t_game *g, int amount)
{
	g->hud.inventory[ITEM_AMMO] += amount;
	show_message(g, "AMMO ADDED TO INVENTORY", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}

static int	magazine_capacity(int weapon)
{
	if (weapon == 1)
		return (BLASTER_MAGAZINE_SIZE);
	return (PISTOL_MAGAZINE_SIZE);
}

void	reload_weapon(t_game *g)
{
	int	weapon;
	int	needed;
	int	loaded;

	weapon = g->hud.selected_weapon;
	needed = magazine_capacity(weapon) - g->hud.magazine[weapon];
	if (needed <= 0)
		return (show_message(g, "MAGAZINE FULL", 1.0));
	if (g->hud.inventory[ITEM_AMMO] <= 0)
		return (show_message(g, "NO RESERVE AMMO", MESSAGE_DISPLAY_TIME));
	loaded = needed;
	if (loaded > g->hud.inventory[ITEM_AMMO])
		loaded = g->hud.inventory[ITEM_AMMO];
	g->hud.inventory[ITEM_AMMO] -= loaded;
	g->hud.magazine[weapon] += loaded;
	show_message(g, "WEAPON RELOADED", 1.0);
	play_sound_effect(g, "switch");
}

void	use_selected_item(t_game *g)
{
	if (g->hud.selected_item == ITEM_AMMO)
		return (reload_weapon(g));
	if (g->hud.selected_item == ITEM_ARTIFACT
		&& g->hud.inventory[ITEM_ARTIFACT] > 0)
	{
		toggle_fly_mode(&g->player, g);
		return ;
	}
	show_message(g, "NOTHING TO USE", MESSAGE_DISPLAY_TIME);
}
