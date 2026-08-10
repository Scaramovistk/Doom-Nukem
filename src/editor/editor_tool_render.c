/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_render.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	put_tool_page_zero(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 932, 126, WHITE, "[1] Wall   [0] Floor");
	mlx_string_put(e->mlx, e->win, 932, 150, WHITE, "[2] Door   [4] Glass");
	mlx_string_put(e->mlx, e->win, 932, 174, WHITE, "[5] Decal  [3] Enemy");
	mlx_string_put(e->mlx, e->win, 932, 198, WHITE, "[6] Health [7] Ammo");
	mlx_string_put(e->mlx, e->win, 932, 222, WHITE, "[;] Object [V] Solid");
	mlx_string_put(e->mlx, e->win, 932, 246, WHITE, "[N] Player [X] Exit");
}

static void	put_tool_page_one(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 932, 126, WHITE, "[8] Key    [B] Locked");
	mlx_string_put(e->mlx, e->win, 932, 150, WHITE, "[P] Secret [L] Elevator");
	mlx_string_put(e->mlx, e->win, 932, 174, WHITE, "[T] Switch [H] Hazard");
	mlx_string_put(e->mlx, e->win, 932, 198, WHITE, "[M] Message [9] Jetpack");
	mlx_string_put(e->mlx, e->win, 932, 222, WHITE, "[Q] Vending [J] Laptop");
	mlx_string_put(e->mlx, e->win, 932, 246, WHITE,
		"[G] Flag   [W] Player west");
}

static void	put_tool_page_two(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 932, 126, WHITE,
		"[S] Player south [E] east");
	mlx_string_put(e->mlx, e->win, 932, 150, WHITE, "[K] Enemy 2 [I] Enemy 3");
	mlx_string_put(e->mlx, e->win, 932, 174, WHITE, "[D] Enemy 4 [C] Enemy 5");
	mlx_string_put(e->mlx, e->win, 932, 198, WHITE, "[A] Deco 1  [F] Deco 2");
	mlx_string_put(e->mlx, e->win, 932, 222, WHITE, "[O] Deco 3  [R] Deco 4");
	mlx_string_put(e->mlx, e->win, 932, 246, WHITE, "[U] Deco 5  [Y] Deco 6");
}

static void	put_tool_page_three(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 932, 126, WHITE, "[Z] Wall deco 1 [-] 2");
	mlx_string_put(e->mlx, e->win, 932, 150, WHITE, "[=] Wall deco 3 [,] 4");
	mlx_string_put(e->mlx, e->win, 932, 174, WHITE, "[.] Wall deco 5 [/] 6");
	mlx_string_put(e->mlx, e->win, 932, 198, WHITE, "['] Outside / void");
}

void	editor_put_tool_labels(t_editor *e)
{
	if (e->tool_page == 0)
		put_tool_page_zero(e);
	else if (e->tool_page == 1)
		put_tool_page_one(e);
	else if (e->tool_page == 2)
		put_tool_page_two(e);
	else
		put_tool_page_three(e);
}
