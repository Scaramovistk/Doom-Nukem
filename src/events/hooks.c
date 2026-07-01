/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:14 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:17 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	setup_hooks(t_game *g)
{
	mlx_loop_hook(g->mlx, game_loop, g);
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
		g->player.key_rotation_move = -1;
	else if (key == KEY_RIGHT)
		g->player.key_rotation_move = 1;
	else if (key == KEY_PAGE_UP)
		g->player.key_pitch_move = -1;
	else if (key == KEY_PAGE_DOWN)
		g->player.key_pitch_move = 1;
	else if (key == KEY_SHIFT)
		g->player.is_running = true;
	else if (key == KEY_CTRL)
	{
		g->player.is_crouching = true;
		g->player.eye_height = PLAYER_CROUCH_HEIGHT;
	}
	else if (key == KEY_SPACE)
		jump_player(&g->player);
	else if (key == KEY_E)
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
		g->player.key_rotation_move = 0;
	else if (key == KEY_PAGE_UP || key == KEY_PAGE_DOWN)
		g->player.key_pitch_move = 0;
	else if (key == KEY_SHIFT)
		g->player.is_running = false;
	else if (key == KEY_CTRL)
	{
		g->player.is_crouching = false;
		g->player.eye_height = PLAYER_STAND_HEIGHT;
	}
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	t_game	*g;
	int		delta_x;
	int		delta_y;

	g = (t_game *)param;
	delta_x = x - (WIN_WIDTH / 2);
	delta_y = y - (WIN_HEIGHT / 2);
	if (delta_x == 0 && delta_y == 0)
		return (0);
	g->player.rotation_move = (double)delta_x;
	g->player.pitch_move = (double)-delta_y;
	g->player.mouse_move_pending = true;
	g->player.mouse.x = x;
	g->player.mouse.y = y;
	move_mouse(g->mlx, g->mlx_win);
	return (0);
}
