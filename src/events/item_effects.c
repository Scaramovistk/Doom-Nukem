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
	g->hud.health += amount;
	if (g->hud.health > g->hud.max_health)
		g->hud.health = g->hud.max_health;
	show_message(g, "HEALTH PACK PICKED UP", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}

void	apply_ammo_pickup(t_game *g, int amount)
{
	g->hud.ammo += amount;
	show_message(g, "AMMO PACK PICKED UP", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}

bool	consume_key(t_game *g)
{
	if (g->hud.inventory[ITEM_KEY] <= 0)
		return (false);
	g->hud.inventory[ITEM_KEY]--;
	return (true);
}

void	use_selected_item(t_game *g)
{
	if (g->hud.selected_item != ITEM_ARTIFACT
		|| g->hud.inventory[ITEM_ARTIFACT] <= 0)
	{
		show_message(g, "NOTHING TO USE", MESSAGE_DISPLAY_TIME);
		return ;
	}
	g->hud.inventory[ITEM_ARTIFACT]--;
	g->hud.ammo += ARTIFACT_AMMO_BONUS;
	g->hud.health = g->hud.max_health;
	show_message(g, "ARTIFACT USED: OVERDRIVE", MESSAGE_DISPLAY_TIME);
	play_sound_effect(g, "pickup");
}
