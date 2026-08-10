/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_keyboard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	sector_key(int key, t_editor *e)
{
	char	command[LINE_SIZE];

	if (key == KEY_PAGE_UP)
		editor_adjust_sector(e, 0);
	else if (key == KEY_PAGE_DOWN)
		editor_adjust_sector(e, 1);
	else if (key == KEY_LEFT)
		editor_adjust_sector(e, 2);
	else if (key == KEY_RIGHT)
		editor_adjust_sector(e, 3);
	else if (key == KEY_SPACE)
		editor_adjust_sector(e, 4);
	else if (key == KEY_TAB && e->selected_x >= 0)
	{
		snprintf(command, sizeof(command), "cell %d %d %c", e->selected_x,
			e->selected_y, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[e->sector]);
		editor_run_command(e, command, "Sector assigned to selected cell");
	}
	else
		return (false);
	return (true);
}

void	editor_project_key(int key, t_editor *e)
{
	if (key == KEY_HOME)
		editor_run_command(e, "save", "Source project saved");
	else if (key == KEY_END)
		editor_run_command(e, "pack", "Packed self-contained .dnk level");
	else if (key == KEY_DELETE)
		editor_run_command(e, "validate", "Validation finished; see output");
	else if (key == KEY_ENTER)
		editor_action_at_selection(e);
	else if (key == KEY_BACKSPACE)
		editor_texture_preset(e);
	else
		sector_key(key, e);
}

bool	editor_select_tool(int key, t_editor *e)
{
	char	tool;

	tool = editor_tool_from_key(key);
	if (!tool)
		return (false);
	e->tool = tool;
	editor_apply_selected_tool(e);
	return (true);
}
