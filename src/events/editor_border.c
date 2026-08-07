/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_border.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/editor.h"

bool	editor_border_valid(t_editor_doc *doc)
{
	char	**map;
	bool	valid;
	int		y;

	map = malloc(sizeof(char *) * doc->map_height);
	if (!map)
		return (false);
	y = 0;
	while (y < doc->map_height)
	{
		map[y] = doc->cub[doc->map_start + y];
		y++;
	}
	valid = ft_solid_map_border(map, doc->map_height, doc->map_width);
	free(map);
	return (valid);
}
