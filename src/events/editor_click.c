/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_click.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	select_panel_tool(t_editor *e, int x, int y)
{
	const char	*tools;
	int			index;
	int			row;
	int			column;

	if (y >= 76 && y < 108)
	{
		if (x < 1080)
			e->tool_page = (e->tool_page + 3) % 4;
		else
			e->tool_page = (e->tool_page + 1) % 4;
		return (true);
	}
	if (y < 112 || y >= 280)
		return (false);
	tools = "10245367vVNX8BPLTHM9QJGWSEKIDCabcdefghijkl_";
	row = (y - 112) / 28;
	column = (x >= 1080);
	index = e->tool_page * 12 + row * 2 + column;
	if (row < 6 && index < 43)
		e->tool = tools[index];
	return (true);
}

static bool	sector_panel_click(t_editor *e, int x, int y)
{
	if (y >= 304 && y < 332)
	{
		if (x < 1080 && e->sector > 0)
			e->sector--;
		else if (x >= 1080 && e->sector < SECTOR_MAX - 1)
			e->sector++;
		return (true);
	}
	if (y >= 332 && y < 356 && e->selected_x >= 0)
		return (editor_project_key(KEY_C, e), true);
	if (y >= 356 && y < 380)
		return (editor_adjust_sector(e, x >= 1080), true);
	if (y >= 380 && y < 404)
		return (editor_adjust_sector(e, 2 + (x >= 1080)), true);
	return (false);
}

static bool	action_panel_click(t_editor *e, int x, int y)
{
	if (y >= 404 && y < 432)
	{
		if (x < 1080)
			editor_action_at_selection(e);
		else
			editor_adjust_sector(e, 4);
		return (true);
	}
	if (y >= 474 && y < 502)
		return (editor_texture_preset(e), true);
	if (y >= 502 && y < 530)
		return (editor_project_key(KEY_S, e), true);
	if (y >= 530 && y < 558)
		return (editor_project_key(KEY_P, e), true);
	if (y >= 558 && y < 586)
		return (editor_project_key(KEY_K, e), true);
	return (false);
}

bool	editor_panel_click(t_editor *e, int x, int y)
{
	if (select_panel_tool(e, x, y))
		return (true);
	if (sector_panel_click(e, x, y))
		return (true);
	return (action_panel_click(e, x, y));
}
