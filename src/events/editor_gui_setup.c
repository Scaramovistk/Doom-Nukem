/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_editor_state(t_editor *e)
{
	e->map_x = 32;
	e->map_y = 84;
	e->tool = '1';
	e->sector = 0;
	e->selected_x = -1;
	e->selected_y = -1;
	e->texture_page = 0;
	e->tool_page = 0;
	e->running = true;
	e->redraw = true;
	e->confirm_close = false;
}

void	editor_setup(t_editor *e)
{
	int	x_size;
	int	y_size;

	x_size = 850 / e->doc.map_width;
	y_size = 650 / e->doc.map_height;
	e->cell_size = x_size;
	if (y_size < x_size)
		e->cell_size = y_size;
	if (e->cell_size > 32)
		e->cell_size = 32;
	if (e->cell_size < 4)
		e->cell_size = 4;
	set_editor_state(e);
	ft_strlcpy(e->status, "Click a tool, then paint the map", HUD_MESSAGE_LEN);
	if (!editor_border_valid(&e->doc))
		ft_strlcpy(e->status, "Invalid border: repair every edge with walls",
			HUD_MESSAGE_LEN);
}
