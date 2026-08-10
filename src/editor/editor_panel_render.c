/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_panel_render.c                              :+:      :+:    :+:   */
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
	mlx_string_put(e->mlx, e->win, 932, 342, WHITE,
		"TAB assign sector to cell");
	mlx_string_put(e->mlx, e->win, 932, 366, WHITE, "PGUP floor  PGDN ceiling");
	mlx_string_put(e->mlx, e->win, 932, 390, WHITE,
		"LEFT/RIGHT slopes  SPACE light");
	mlx_string_put(e->mlx, e->win, 932, 414, WHITE, "ENTER add door action");
	mlx_string_put(e->mlx, e->win, 932, 454, GREEN, "TEXTURES / PROJECT");
	mlx_string_put(e->mlx, e->win, 932, 486, WHITE,
		"BACKSPACE Texture preset");
	mlx_string_put(e->mlx, e->win, 932, 510, WHITE, "HOME Save   END Pack");
	mlx_string_put(e->mlx, e->win, 932, 534, WHITE,
		"DELETE Validate  ESC Close");
	mlx_string_put(e->mlx, e->win, 932, 558, WHITE, "INSERT Fullscreen/window");
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
