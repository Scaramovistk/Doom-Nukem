/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/editor.h"

static void	run_editor_command(t_editor *e, char *command, char *status)
{
	if ((!ft_strncmp(command, "save", 4) || !ft_strncmp(command, "pack", 4)
			|| !ft_strncmp(command, "validate", 8))
		&& !editor_border_valid(&e->doc))
	{
		ft_strlcpy(e->status, "Cannot save: border must be solid walls",
			HUD_MESSAGE_LEN);
		e->redraw = true;
		return ;
	}
	editor_command(&e->doc, command);
	ft_strlcpy(e->status, status, HUD_MESSAGE_LEN);
	e->redraw = true;
}

static void	action_at_selection(t_editor *e)
{
	char	command[LINE_SIZE];

	if (e->selected_x < 0)
		return ((void)ft_strlcpy(e->status, "Select a trigger cell first",
				HUD_MESSAGE_LEN));
	snprintf(command, sizeof(command),
		"action add %d %d 0.0 BLOCK %d %d DOOR", e->selected_x,
		e->selected_y, e->selected_x, e->selected_y);
	run_editor_command(e, command, "Door action added at selected cell");
}

static void	adjust_sector(t_editor *e, int property)
{
	t_sector	s;
	char		command[LINE_SIZE];
	int			i;
	int			id;

	s = (t_sector){0.0, 1.0, 0.0, 0.0, 255, true, false, 0, 0};
	i = 0;
	while (i < e->doc.sector_count)
	{
		if (sscanf(e->doc.sectors[i], "SECTOR %d %lf %lf %lf %lf %d", &id,
				&s.floor_z, &s.ceil_z, &s.slope_x, &s.slope_y, &s.light) == 6
			&& id == e->sector)
			break ;
		i++;
	}
	if (property == 0 && s.floor_z + 0.1 < s.ceil_z)
		s.floor_z += 0.1;
	else if (property == 1)
		s.ceil_z += 0.1;
	else if (property == 2)
		s.slope_x += 0.05;
	else if (property == 3)
		s.slope_y += 0.05;
	else if (property == 4)
		s.light = (s.light + 224) % 256;
	snprintf(command, sizeof(command), "sector %d %.2f %.2f %.2f %.2f %d",
		e->sector, s.floor_z, s.ceil_z, s.slope_x, s.slope_y, s.light);
	run_editor_command(e, command, "Sector property updated");
}

static void	texture_preset(t_editor *e)
{
	char			*keys[6];
	char			*blue[6];
	char			*doom[6];
	char			command[LINE_SIZE];
	char			**files;
	int				i;

	memcpy(keys, (char *[6]){"NO", "SO", "EA", "WE", "F", "C"},
		sizeof(keys));
	memcpy(blue, (char *[6]){"blue1.xpm", "blue4.xpm", "blue3.xpm",
		"blue2.xpm", "floor_stone.xpm", "ceiling_panel.xpm"}, sizeof(blue));
	memcpy(doom, (char *[6]){"level1_concrete.xpm", "uac_red_wall.xpm",
		"uac_metal_wall.xpm", "level1_metal_wall.xpm", "toxic_floor.xpm",
		"uac_ceiling_light.xpm"}, sizeof(doom));

	e->page = (e->page + 1) % 2;
	files = blue;
	if (e->page == 1)
		files = doom;
	i = 0;
	while (i < 6)
	{
		snprintf(command, sizeof(command), "texture %s assets/images/textures/%s/%s",
			keys[i], (char *[2]){"blue", "doom"}[e->page], files[i]);
		editor_command(&e->doc, command);
		i++;
	}
	ft_strlcpy(e->status, "Texture set changed", HUD_MESSAGE_LEN);
}

static void	project_key(int key, t_editor *e)
{
	if (key == KEY_S)
		run_editor_command(e, "save", "Source project saved");
	else if (key == KEY_P)
		run_editor_command(e, "pack", "Packed self-contained .dnk level");
	else if (key == KEY_K)
		run_editor_command(e, "validate", "Validation finished; see output");
	else if (key == KEY_A)
		action_at_selection(e);
	else if (key == KEY_F)
		adjust_sector(e, 0);
	else if (key == KEY_R)
		adjust_sector(e, 1);
	else if (key == KEY_G)
		adjust_sector(e, 2);
	else if (key == KEY_H)
		adjust_sector(e, 3);
	else if (key == KEY_L)
		adjust_sector(e, 4);
	else if (key == KEY_T)
		texture_preset(e);
	else if (key == KEY_C && e->selected_x >= 0)
	{
		char command[LINE_SIZE];

		snprintf(command, sizeof(command), "cell %d %d %c", e->selected_x,
			e->selected_y, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[e->sector]);
		run_editor_command(e, command, "Sector assigned to selected cell");
	}
}

static bool	select_tool(int key, t_editor *e)
{
	if (key == KEY_Z)
		e->tool = '0';
	else if (key == KEY_1)
		e->tool = '1';
	else if (key == KEY_2)
		e->tool = '2';
	else if (key == KEY_3)
		e->tool = '3';
	else if (key == KEY_4)
		e->tool = '4';
	else if (key == KEY_5)
		e->tool = '5';
	else if (key == KEY_6)
		e->tool = '6';
	else if (key == KEY_7)
		e->tool = '7';
	else if (key == KEY_V)
		e->tool = 'V';
	else if (key == KEY_N)
		e->tool = 'N';
	else if (key == KEY_X)
		e->tool = 'X';
	else
		return (false);
	return (true);
}

static bool	panel_click(t_editor *e, int x, int y)
{
	char	*tools[6];
	int		row;
	int		column;

	memcpy(tools, (char *[6]){"10", "24", "53", "67", "VV", "NX"},
		sizeof(tools));
	if (y >= 112 && y < 280)
	{
		row = (y - 112) / 28;
		column = (x >= 1080);
		if (row < 6)
			e->tool = tools[row][column];
		return (true);
	}
	if (y >= 304 && y < 332)
	{
		if (x < 1080 && e->sector > 0)
			e->sector--;
		else if (x >= 1080 && e->sector < SECTOR_MAX - 1)
			e->sector++;
		return (true);
	}
	if (y >= 332 && y < 356 && e->selected_x >= 0)
		return (project_key(KEY_C, e), true);
	if (y >= 356 && y < 380)
		return (adjust_sector(e, x >= 1080), true);
	if (y >= 380 && y < 404)
		return (adjust_sector(e, 2 + (x >= 1080)), true);
	if (y >= 404 && y < 432)
	{
		if (x < 1080)
			action_at_selection(e);
		else
			adjust_sector(e, 4);
		return (true);
	}
	if (y >= 474 && y < 502)
		return (texture_preset(e), true);
	if (y >= 502 && y < 530)
		return (project_key(KEY_S, e), true);
	if (y >= 530 && y < 558)
		return (project_key(KEY_P, e), true);
	if (y >= 558 && y < 586)
		return (project_key(KEY_K, e), true);
	return (false);
}

int	editor_key(int key, t_editor *e)
{
	if (key == KEY_ESC)
		return (editor_exit(e));
	if (key == KEY_F11 || key == KEY_0)
		toggle_fullscreen(e->mlx, e->win, NULL);
	else if (key == KEY_LEFT_BRACKET && e->sector > 0)
		e->sector--;
	else if (key == KEY_RIGHT_BRACKET && e->sector < SECTOR_MAX - 1)
		e->sector++;
	else if (!select_tool(key, e))
		project_key(key, e);
	e->redraw = true;
	return (0);
}

int	editor_mouse(int button, int x, int y, t_editor *e)
{
	char	command[LINE_SIZE];
	int		map_x;
	int		map_y;

	if (button != MOUSE_LEFT)
		return (0);
	if (x >= 918 && panel_click(e, x, y))
		return (e->redraw = true, 0);
	map_x = (x - e->map_x) / e->cell_size;
	map_y = (y - e->map_y) / e->cell_size;
	if (x < e->map_x || y < e->map_y || map_x < 0 || map_y < 0
		|| map_x >= e->doc.map_width || map_y >= e->doc.map_height)
		return (0);
	e->selected_x = map_x;
	e->selected_y = map_y;
	if ((map_x == 0 || map_y == 0 || map_x == e->doc.map_width - 1
			|| map_y == e->doc.map_height - 1) && e->tool != '1')
	{
		ft_strlcpy(e->status, "Border cells are locked to solid walls",
			HUD_MESSAGE_LEN);
		e->redraw = true;
		return (0);
	}
	snprintf(command, sizeof(command), "set %d %d %c", map_x, map_y, e->tool);
	run_editor_command(e, command, "Map cell updated");
	return (0);
}
