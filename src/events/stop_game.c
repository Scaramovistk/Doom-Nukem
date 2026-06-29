/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:21 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:23 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	stop_game(void *param)
{
	t_game	*g;
	int		exit_status;

	g = (t_game *)param;
	exit_status = g->exit_status;
	free_all(g);
	exit(exit_status);
}
