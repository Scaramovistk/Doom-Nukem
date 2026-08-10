/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui_labels.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	put_geometry_labels(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 24, 34, 0xFF8A35,
		"DOOM-NUKEM LEVEL EDITOR");
	mlx_string_put(e->mlx, e->win, 932, 94, WHITE, "<  TOOL PAGE  >");
	editor_put_tool_labels(e);
}

static void	put_action_labels(t_editor *e)
{
	mlx_string_put(e->mlx, e->win, 932, 286, GREEN, "SECTORS / ACTIONS");
	mlx_string_put(e->mlx, e->win, 932, 318, WHITE, "[ / ] Sector id");
	mlx_string_put(e->mlx, e->win, 932, 342, WHITE, "C assign sector to cell");
	mlx_string_put(e->mlx, e->win, 932, 366, WHITE, "F floor  R ceiling");
	mlx_string_put(e->mlx, e->win, 932, 390, WHITE, "G/H slopes  L light");
	mlx_string_put(e->mlx, e->win, 932, 414, WHITE, "A add door action");
	mlx_string_put(e->mlx, e->win, 932, 454, GREEN, "TEXTURES / PROJECT");
	mlx_string_put(e->mlx, e->win, 932, 486, WHITE, "T Texture preset");
	mlx_string_put(e->mlx, e->win, 932, 510, WHITE, "S Save   P Pack");
	mlx_string_put(e->mlx, e->win, 932, 534, WHITE, "K Validate  ESC Close");
	mlx_string_put(e->mlx, e->win, 932, 558, WHITE,
		"0 / F11 Fullscreen/window");
}

void	editor_put_labels(t_editor *e)
{
	char	line[LINE_SIZE];

	put_geometry_labels(e);
	put_action_labels(e);
	snprintf(line, sizeof(line), "Tool: %c  Sector: %d  Texture: %d",
		e->tool, e->sector, e->texture_page + 1);
	mlx_string_put(e->mlx, e->win, 932, 608, YELLOW, line);
	mlx_string_put(e->mlx, e->win, 24, 782, 0xAAB2BF, e->status);
}
