/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_mouse_pos_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:57:07 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:57:10 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef __linux__

int	get_mouse_pos(void *mlx, void *win, int *x, int *y)
{
	mlx_mouse_get_pos(mlx, win, x, y);
	return (0);
}

int	move_mouse(void *mlx, void *win)
{
	mlx_mouse_move(mlx, win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	return (0);
}

#elif defined(__APPLE__)

int	get_mouse_pos(void *mlx, void *win, int *x, int *y)
{
	(void)mlx;
	mlx_mouse_get_pos(win, x, y);
	return (0);
}

int	move_mouse(void *mlx, void *win)
{
	(void)mlx;
	mlx_mouse_move(win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	return (0);
}

#else
# error "Unsupported operating system"
#endif