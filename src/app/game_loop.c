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

static bool	player_is_idle(t_game *g)
{
	return (!g->player.vertical_move && !g->player.lateral_move
		&& !g->player.rotation_move && !g->player.key_rotation_move
		&& !g->player.pitch_move && !g->player.key_pitch_move
		&& !g->player.fly_move && (g->player.on_ground
			|| g->player.is_flying || g->player.is_swimming));
}

static bool	world_is_active(t_game *g)
{
	bool	active[8];

	update_doors(&active[0], g);
	update_audio();
	active[1] = update_projectiles(g);
	active[2] = update_weapon_view(g);
	active[3] = update_enemies(g);
	active[4] = update_proximity_triggers(g);
	active[5] = update_world_events(g);
	active[6] = update_message(g);
	active[7] = update_level_flow(g);
	return (active[0] || active[1] || active[2] || active[3]
		|| active[4] || active[5] || active[6] || active[7]);
}

int	game_loop(t_game *g)
{
	if (g->state == STATE_MENU)
		return (EXIT_SUCCESS);
	if (g->story_visible)
	{
		render(g);
		return (EXIT_SUCCESS);
	}
	update_bsp_visibility(g);
	if (!world_is_active(g) && player_is_idle(g))
		return (EXIT_SUCCESS);
	update_player_pos(&g->player, g);
	update_item_pickups(g);
	update_level_flow(g);
	update_bsp_visibility(g);
	render(g);
	return (EXIT_SUCCESS);
}
