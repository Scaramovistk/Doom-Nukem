/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_mac.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef __APPLE__

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

int	move_mouse_at(void *mlx, void *win, int x, int y)
{
	(void)mlx;
	mlx_mouse_move(win, x, y);
	return (0);
}

int	hide_mouse(void *mlx, void *win)
{
	(void)mlx;
	(void)win;
	mlx_mouse_hide();
	return (0);
}

#endif
