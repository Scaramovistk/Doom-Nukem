/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	editor_run_command(t_editor *e, char *command, char *status)
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

void	editor_action_at_selection(t_editor *e)
{
	char	command[LINE_SIZE];

	if (e->selected_x < 0)
		return ((void)ft_strlcpy(e->status, "Select a trigger cell first",
				HUD_MESSAGE_LEN));
	snprintf(command, sizeof(command),
		"action add %d %d 0.0 BLOCK %d %d DOOR", e->selected_x,
		e->selected_y, e->selected_x, e->selected_y);
	editor_run_command(e, command, "Door action added at selected cell");
}

static void	read_sector(t_editor *e, t_sector *sector)
{
	int	i;
	int	id;

	*sector = (t_sector){0.0, 1.0, 0.0, 0.0, 255, true, false, 0, 0};
	i = 0;
	while (i < e->doc.sector_count)
	{
		if (sscanf(e->doc.sectors[i], "SECTOR %d %lf %lf %lf %lf %d", &id,
				&sector->floor_z, &sector->ceil_z, &sector->slope_x,
				&sector->slope_y, &sector->light) == 6 && id == e->sector)
			return ;
		i++;
	}
}

static void	change_sector_property(t_sector *sector, int property)
{
	if (property == 0 && sector->floor_z + 0.1 < sector->ceil_z)
		sector->floor_z += 0.1;
	else if (property == 1)
		sector->ceil_z += 0.1;
	else if (property == 2)
		sector->slope_x += 0.05;
	else if (property == 3)
		sector->slope_y += 0.05;
	else if (property == 4)
		sector->light = (sector->light + 224) % 256;
}

void	editor_adjust_sector(t_editor *e, int property)
{
	t_sector	sector;
	char		command[LINE_SIZE];

	read_sector(e, &sector);
	change_sector_property(&sector, property);
	snprintf(command, sizeof(command), "sector %d %.2f %.2f %.2f %.2f %d",
		e->sector, sector.floor_z, sector.ceil_z, sector.slope_x,
		sector.slope_y, sector.light);
	editor_run_command(e, command, "Sector property updated");
}
