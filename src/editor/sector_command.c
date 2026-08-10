/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	parse_sector_command(char *command, t_sector *sector, int *id)
{
	int	count;

	count = sscanf(command, "sector %d %lf %lf %lf %lf %d", id,
			&sector->floor_z, &sector->ceil_z, &sector->slope_x,
			&sector->slope_y, &sector->light);
	return (count == 6 && *id >= 0 && *id < SECTOR_MAX
		&& isfinite(sector->floor_z) && isfinite(sector->ceil_z)
		&& isfinite(sector->slope_x) && isfinite(sector->slope_y)
		&& sector->ceil_z > sector->floor_z && sector->light >= 0
		&& sector->light <= 255);
}

bool	replace_sector_line(t_editor_doc *doc, char *line, int id)
{
	int	i;
	int	line_id;

	i = 0;
	while (i < doc->sector_count)
	{
		if (sscanf(doc->sectors[i], "SECTOR %d", &line_id) == 1
			&& line_id == id)
		{
			free(doc->sectors[i]);
			doc->sectors[i] = editor_dup(line);
			doc->dirty = true;
			return (true);
		}
		i++;
	}
	return (false);
}
