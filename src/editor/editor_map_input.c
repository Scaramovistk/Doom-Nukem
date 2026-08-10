/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	editor_key(int key, t_editor *e)
{
	if (key == KEY_ESC)
		return (editor_exit(e));
	if (key == KEY_INSERT)
		toggle_fullscreen(e->mlx, e->win, NULL);
	else if (key == KEY_LEFT_BRACKET && e->sector > 0)
		e->sector--;
	else if (key == KEY_RIGHT_BRACKET && e->sector < SECTOR_MAX - 1)
		e->sector++;
	else if (!editor_select_tool(key, e))
		editor_project_key(key, e);
	e->redraw = true;
	return (0);
}

static bool	border_cell_locked(t_editor *e, int x, int y)
{
	if (x != 0 && y != 0 && x != e->doc.map_width - 1
		&& y != e->doc.map_height - 1)
		return (false);
	if (e->tool == '1')
		return (false);
	ft_strlcpy(e->status, "Border cells are locked to solid walls",
		HUD_MESSAGE_LEN);
	e->redraw = true;
	return (true);
}

void	editor_apply_selected_tool(t_editor *e)
{
	char	command[LINE_SIZE];

	if (e->selected_x < 0 || e->selected_y < 0)
	{
		ft_strlcpy(e->status, "Select a map cell first", HUD_MESSAGE_LEN);
		return ;
	}
	if (border_cell_locked(e, e->selected_x, e->selected_y))
		return ;
	e->blink_visible = true;
	e->blink_time = get_time();
	snprintf(command, sizeof(command), "set %d %d %c", e->selected_x,
		e->selected_y, e->tool);
	editor_run_command(e, command, "Selected cell updated from keyboard");
}

int	editor_mouse(int button, int x, int y, t_editor *e)
{
	int		map_x;
	int		map_y;

	if (button != MOUSE_LEFT)
		return (0);
	if (x >= 918 && editor_panel_click(e, x, y))
		return (e->redraw = true, 0);
	map_x = (x - e->map_x) / e->cell_size;
	map_y = (y - e->map_y) / e->cell_size;
	if (x < e->map_x || y < e->map_y || map_x < 0 || map_y < 0
		|| map_x >= e->doc.map_width || map_y >= e->doc.map_height)
		return (0);
	e->selected_x = map_x;
	e->selected_y = map_y;
	e->blink_visible = true;
	e->blink_time = get_time();
	e->redraw = true;
	ft_strlcpy(e->status, "Cell selected; press a brush key", HUD_MESSAGE_LEN);
	return (0);
}
