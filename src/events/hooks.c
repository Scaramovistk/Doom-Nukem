/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:14 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:17 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	setup_hooks(t_game *g)
{
	mlx_loop_hook(g->mlx, game_loop, g);
	if (MODE_BONUS)
		mlx_hook(g->mlx_win, MOUSE_MOVE, MOUSE_MOVE_MASK, mouse_move, g);
	mlx_hook(g->mlx_win, CLIENT_MESSAGE, STRUCTURE_NOTIFY_MASK, stop_game, g);
	mlx_hook(g->mlx_win, KEY_PRESS, KEY_PRESS_MASK, pressed, g);
	mlx_hook(g->mlx_win, KEY_RELEASE, KEY_RELEASE_MASK, released, g);
}

int	pressed(int key, t_game *g)
{
	if (key == KEY_ESC)
		stop_game(g);
	if (key == KEY_W)
		g->player.vertical_move = 1;
	else if (key == KEY_S)
		g->player.vertical_move = -1;
	else if (key == KEY_A)
		g->player.lateral_move = -1;
	else if (key == KEY_D)
		g->player.lateral_move = 1;
	else if (key == KEY_LEFT)
		g->player.rotation_move = -1;
	else if (key == KEY_RIGHT)
		g->player.rotation_move = 1;
	else if (key == KEY_SPACE)
		toggle_door(g);
	return (0);
}

int	released(int key, t_game *g)
{
	if (key == KEY_W || key == KEY_S)
		g->player.vertical_move = 0;
	else if (key == KEY_A || key == KEY_D)
		g->player.lateral_move = 0;
	else if (key == KEY_LEFT || key == KEY_RIGHT)
		g->player.rotation_move = 0;
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	void	*win;
	void	*mlx;
	int		mouse_x;
	t_game	*g;

	g = (t_game *)param;
	win = g->mlx_win;
	mlx = g->mlx;
	get_mouse_pos(mlx, win, &x, &y);
	mouse_x = g->player.mouse.x;
	if (0 < x && x < WIN_WIDTH && 0 < y && y < WIN_HEIGHT)
	{
		if (x > mouse_x + 1)
			g->player.rotation_move = 2;
		else if (x < mouse_x - 1)
			g->player.rotation_move = -2;
		else
			g->player.rotation_move = 0;
		g->player.mouse.x = x;
	}
	else
		g->player.rotation_move = 0;
	return (0);
}
