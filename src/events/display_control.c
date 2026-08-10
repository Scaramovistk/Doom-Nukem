/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef __linux__

void	end_display_loop(void *mlx)
{
	mlx_loop_end(mlx);
}

void	destroy_display(void *mlx)
{
	mlx_destroy_display(mlx);
}

#elif defined(__APPLE__)

void	end_display_loop(void *mlx)
{
	(void)mlx;
}

void	destroy_display(void *mlx)
{
	(void)mlx;
}

#endif
