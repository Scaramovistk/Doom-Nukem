/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   document_map_edit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	valid_map_token(char token)
{
	return (ft_strchr("0123456789NWSETHMXKLPIDCVBGQJabcdefghijkl_v",
			token) != NULL);
}

static bool	expand_map_row(t_editor_doc *doc, int y)
{
	char	*row;
	char	*expanded;
	int		length;

	row = doc->cub[doc->map_start + y];
	length = ft_strlen(row);
	if (length >= doc->map_width)
		return (true);
	expanded = malloc(doc->map_width + 1);
	if (!expanded)
		return (false);
	ft_memset(expanded, ' ', doc->map_width);
	ft_memcpy(expanded, row, length);
	expanded[doc->map_width] = '\0';
	free(row);
	doc->cub[doc->map_start + y] = expanded;
	return (true);
}

void	set_map_cell(t_editor_doc *doc, char *command)
{
	int		x;
	int		y;
	char	token;

	if (sscanf(command, "set %d %d %c", &x, &y, &token) != 3 || x < 0 || y < 0
		|| y >= doc->map_height || x >= doc->map_width
		|| !valid_map_token(token))
		return ((void) printf("Usage: set <x> <y> <map-token>\n"));
	if (!expand_map_row(doc, y))
		return ((void) printf("Unable to extend map row.\n"));
	if ((x == 0 || y == 0 || x == doc->map_width - 1 || y == doc->map_height
			- 1) && token != '1')
		return ((void) printf("Border cells must remain solid walls ('1').\n"));
	if (token == '_')
		token = ' ';
	doc->cub[doc->map_start + y][x] = token;
	doc->dirty = true;
}

bool	line_has_key(const char *line, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (!ft_strncmp(line, key, len) && (line[len] == ' '
			|| line[len] == '\t'));
}

bool	insert_line(char **lines, int *count, int index, const char *text)
{
	int	i;

	if (*count >= DNK_MAX_LINES || index < 0 || index > *count)
		return (false);
	i = *count;
	while (i > index)
	{
		lines[i] = lines[i - 1];
		i--;
	}
	lines[index] = editor_dup(text);
	if (!lines[index])
		return (false);
	(*count)++;
	return (true);
}
