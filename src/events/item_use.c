/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_use.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
