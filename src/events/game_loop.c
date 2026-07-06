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
	render(g);
	mlx_loop(g->mlx);
	return ;
}

int	game_loop(t_game *g)
{
	bool	door_updated;

	update_doors(&door_updated, g);
	if (!door_updated && !g->player.vertical_move && !g->player.lateral_move
		&& !g->player.rotation_move && !g->player.key_rotation_move
		&& !g->player.pitch_move && !g->player.key_pitch_move
		&& !g->player.fly_move
		&& (g->player.on_ground || g->player.is_flying
			|| g->player.is_swimming))
		return (EXIT_SUCCESS);
	update_player_pos(&g->player, g);
	update_item_pickups(g);
	render(g);
	return (EXIT_SUCCESS);
}
