/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_items.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	select_item(t_game *g, int item)
{
	g->hud.selected_item = item;
	if (g->hud.inventory[item] > 0)
		show_message(g, "ARTIFACT SELECTED", 1.0);
	else
		show_message(g, "EMPTY ARTIFACT SLOT", 1.0);
}

static void	cycle_weapon(t_game *g)
{
	g->hud.selected_weapon = (g->hud.selected_weapon + 1) % WEAPON_NB;
	if (g->hud.selected_weapon == 0)
		show_message(g, "PISTOL", 1.0);
	else
		show_message(g, "BLASTER", 1.0);
}

void	handle_action_key(int key, t_game *g)
{
	if (key == KEY_E)
		interact(g);
	else if (key == KEY_F && g->hud.inventory[ITEM_ARTIFACT] > 0)
		toggle_fly_mode(&g->player, g);
	else if (key == KEY_F)
		show_message(g, "JETPACK REQUIRED", MESSAGE_DISPLAY_TIME);
	else if (key == KEY_Q)
		cycle_weapon(g);
	else if (key == KEY_R)
		reload_weapon(g);
	else if (key >= KEY_1 && key <= KEY_4)
		select_item(g, key - KEY_1);
	else if (key == KEY_ENTER)
		use_selected_item(g);
}
