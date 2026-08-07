/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
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
