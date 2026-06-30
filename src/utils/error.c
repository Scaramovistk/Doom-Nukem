/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:19:23 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:19:27 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	error(char *description, t_game *g)
{
	ft_write_fd("Error : ", STDERR_FILENO);
	ft_write_fd(description, STDERR_FILENO);
	ft_write_fd("\n", STDERR_FILENO);
	g->exit_status = EXIT_FAILURE;
	stop_game(g);
}
