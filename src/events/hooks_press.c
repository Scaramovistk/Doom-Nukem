/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_press.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	press_crouch(t_game *g)
{
	g->player.is_crouching = true;
	g->player.eye_height = PLAYER_CROUCH_HEIGHT;
	if (g->player.is_flying || g->player.is_swimming)
		g->player.fly_move = -1;
}

static void	press_space(t_game *g)
{
	if (g->player.is_flying || g->player.is_swimming)
		g->player.fly_move = 1;
	else
		jump_player(&g->player);
}

static bool	handle_move_key(int key, t_game *g)
{
	if (key == KEY_W || key == KEY_UP)
		g->player.vertical_move = 1;
	else if (key == KEY_S || key == KEY_DOWN)
		g->player.vertical_move = -1;
	else if (key == KEY_A || key == KEY_D)
		g->player.lateral_move = 1 - 2 * (key == KEY_A);
	else if (key == KEY_LEFT || key == KEY_RIGHT)
		g->player.key_rotation_move = 1 - 2 * (key == KEY_LEFT);
	else if (key == KEY_PAGE_UP || key == KEY_PAGE_DOWN)
		g->player.key_pitch_move = 1 - 2 * (key == KEY_PAGE_UP);
	else if (key == KEY_SHIFT)
		g->player.is_running = true;
	else if (key == KEY_CTRL)
		press_crouch(g);
	else if (key == KEY_SPACE)
		press_space(g);
	else
		return (false);
	return (true);
}

static void	handle_story_key(int key, t_game *g)
{
	if (key != KEY_ENTER)
		return ;
	g->story_visible = false;
	if (!g->story_is_debrief)
		return ;
	if (g->level.next_level[0])
		load_next_level(g);
	else
		stop_game(g);
}

int	pressed(int key, t_game *g)
{
	if (key == KEY_ESC)
		stop_game(g);
	if (key == KEY_F11 || key == KEY_0)
	{
		toggle_fullscreen(g->mlx, g->mlx_win, g);
		if (g->state == STATE_MENU)
			render_menu(g);
		else
			render(g);
		return (0);
	}
	if (g->state == STATE_MENU)
		return (menu_key(key, g));
	if (g->story_visible)
		return (handle_story_key(key, g), 0);
	if (!handle_move_key(key, g))
		handle_action_key(key, g);
	return (0);
}
