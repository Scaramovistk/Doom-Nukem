/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_keys.c                                      :+:      :+:    :+:   */
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

	if (key == KEY_F)
		editor_adjust_sector(e, 0);
	else if (key == KEY_R)
		editor_adjust_sector(e, 1);
	else if (key == KEY_G)
		editor_adjust_sector(e, 2);
	else if (key == KEY_H)
		editor_adjust_sector(e, 3);
	else if (key == KEY_L)
		editor_adjust_sector(e, 4);
	else if (key == KEY_C && e->selected_x >= 0)
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
	if (key == KEY_S)
		editor_run_command(e, "save", "Source project saved");
	else if (key == KEY_P)
		editor_run_command(e, "pack", "Packed self-contained .dnk level");
	else if (key == KEY_K)
		editor_run_command(e, "validate", "Validation finished; see output");
	else if (key == KEY_A)
		editor_action_at_selection(e);
	else if (key == KEY_T)
		editor_texture_preset(e);
	else
		sector_key(key, e);
}

bool	editor_select_tool(int key, t_editor *e)
{
	const int	keys[11] = {KEY_Z, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
		KEY_6, KEY_7, KEY_V, KEY_N, KEY_X};
	const char	tools[12] = "01234567VNX";
	int			i;

	i = 0;
	while (i < 11)
	{
		if (key == keys[i])
		{
			e->tool = tools[i];
			return (true);
		}
		i++;
	}
	return (false);
}
