/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:04 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:08 by ugerkens         ###   ########.fr       */
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
		&& !g->player.rotation_move)
		return (EXIT_SUCCESS);
	update_player_pos(&g->player, g);
	render(g);
	return (EXIT_SUCCESS);
}
