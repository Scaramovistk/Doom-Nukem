/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_mouse_linux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
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

int	move_mouse_at(void *mlx, void *win, int x, int y)
{
	mlx_mouse_move(mlx, win, x, y);
	return (0);
}

int	hide_mouse(void *mlx, void *win)
{
	mlx_mouse_hide(mlx, win);
	return (0);
}

#endif
