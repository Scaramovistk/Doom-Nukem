/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	apply_texture_set(t_editor *e, char **keys, char **files,
		char *folder)
{
	char	command[LINE_SIZE];
	int		i;

	i = 0;
	while (i < 6)
	{
		snprintf(command, sizeof(command),
			"texture %s assets/images/textures/%s/%s", keys[i], folder,
			files[i]);
		editor_command(&e->doc, command);
		i++;
	}
}

void	editor_texture_preset(t_editor *e)
{
	char	*keys[6];
	char	*blue[6];
	char	*doom[6];

	memcpy(keys, (char *[6]){"NO", "SO", "EA", "WE", "F", "C"},
		sizeof(keys));
	memcpy(blue, (char *[6]){"blue1.xpm", "blue4.xpm", "blue3.xpm",
		"blue2.xpm", "floor_stone.xpm", "ceiling_panel.xpm"}, sizeof(blue));
	memcpy(doom, (char *[6]){"level1_concrete.xpm", "uac_red_wall.xpm",
		"uac_metal_wall.xpm", "level1_metal_wall.xpm", "toxic_floor.xpm",
		"uac_ceiling_light.xpm"}, sizeof(doom));
	e->texture_page = (e->texture_page + 1) % 2;
	if (e->texture_page == 1)
		apply_texture_set(e, keys, doom, "doom");
	else
		apply_texture_set(e, keys, blue, "blue");
	ft_strlcpy(e->status, "Texture set changed", HUD_MESSAGE_LEN);
}
