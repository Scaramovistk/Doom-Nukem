/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	replace_texture(t_editor_doc *doc, const char *key,
		const char *line)
{
	int	i;

	i = 0;
	while (i < doc->map_start)
	{
		if (line_has_key(doc->cub[i], key))
		{
			free(doc->cub[i]);
			doc->cub[i] = editor_dup(line);
			doc->dirty = true;
			return (true);
		}
		i++;
	}
	return (false);
}

void	set_texture(t_editor_doc *doc, char *command)
{
	char	key[16];
	char	value[LINE_SIZE];
	char	line[LINE_SIZE];

	if (sscanf(command, "texture %15s %299[^\n]", key, value) != 2)
		return ((void) printf("Usage: texture <header-key> <path-or-RGB>\n"));
	if (snprintf(line, sizeof(line), "%s %s", key, value)
		>= (int) sizeof(line))
		return ((void) printf("Texture line is too long.\n"));
	if (replace_texture(doc, key, line))
		return ;
	if (!insert_line(doc->cub, &doc->cub_count, doc->map_start, line))
		return ((void) printf("Unable to add header line.\n"));
	doc->map_start++;
	doc->dirty = true;
}
