/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:04 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:08 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	start_game(t_game *g)
{
	if (g->state == STATE_MENU)
		render_menu(g);
	else
	{
		start_level_flow(g);
		start_background_music(g);
		render(g);
	}
	mlx_loop(g->mlx);
	return ;
}

static bool	update_weapon_view(t_game *g)
{
	if (g->hud.weapon_flash <= 0.0)
		return (false);
	g->hud.weapon_flash -= g->delta_time;
	if (g->hud.weapon_flash < 0.0)
		g->hud.weapon_flash = 0.0;
	return (true);
}

int	game_loop(t_game *g)
{
	bool	door_updated;
	bool	zone_active;
	bool	event_active;
	bool	message_active;
	bool	level_active;
	bool	projectile_active;
	bool	weapon_active;
	bool	enemy_active;

	if (g->state == STATE_MENU)
		return (EXIT_SUCCESS);
	update_doors(&door_updated, g);
	update_audio();
	projectile_active = update_projectiles(g);
	weapon_active = update_weapon_view(g);
	enemy_active = update_enemies(g);
	zone_active = update_proximity_triggers(g);
	event_active = update_world_events(g);
	message_active = update_message(g);
	level_active = update_level_flow(g);
	if (!door_updated && !zone_active && !event_active && !message_active
		&& !level_active && !projectile_active && !weapon_active
		&& !enemy_active && !g->player.vertical_move
		&& !g->player.lateral_move && !g->player.rotation_move
		&& !g->player.key_rotation_move && !g->player.pitch_move
		&& !g->player.key_pitch_move && !g->player.fly_move
		&& (g->player.on_ground || g->player.is_flying
			|| g->player.is_swimming))
		return (EXIT_SUCCESS);
	update_player_pos(&g->player, g);
	update_item_pickups(g);
	update_level_flow(g);
	render(g);
	return (EXIT_SUCCESS);
}
