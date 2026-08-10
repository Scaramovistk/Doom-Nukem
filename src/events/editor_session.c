/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_session.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/04 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	*editor_dup(const char *text)
{
	char	*copy;
	size_t	len;

	copy = ft_strdup(text);
	if (!copy)
		return (NULL);
	len = ft_strlen(copy);
	while (len && (copy[len - 1] == '\n' || copy[len - 1] == '\r'))
		copy[--len] = '\0';
	return (copy);
}

void	free_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
}

void	free_document(t_editor_doc *doc)
{
	free_lines(doc->cub, doc->cub_count);
	free_lines(doc->sectors, doc->sector_count);
	doc->cub_count = 0;
	doc->sector_count = 0;
}

void	editor_close(t_editor_doc *doc)
{
	free_document(doc);
}

bool	read_lines(const char *path, char **lines, int *count)
{
	char	*line;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	line = get_next_line(fd, false);
	while (line && *count < DNK_MAX_LINES)
	{
		lines[*count] = editor_dup(line);
		free(line);
		if (!lines[*count])
			return (close(fd), false);
		(*count)++;
		line = get_next_line(fd, false);
	}
	if (line)
		return (free(line), close(fd), false);
	free(line);
	close(fd);
	return (*count > 0);
}
