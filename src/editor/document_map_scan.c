/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   document_map_scan.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	is_map_line(const char *line)
{
	const char	*valid;
	int			i;
	bool		non_space;

	valid = "0123456789 NWSETHMXKLPIDCVBGQJabcdefghijklv";
	i = 0;
	non_space = false;
	while (line[i])
	{
		if (!ft_strchr(valid, line[i]))
			return (false);
		if (line[i] != ' ')
			non_space = true;
		i++;
	}
	return (non_space);
}

bool	scan_map(t_editor_doc *doc)
{
	int	i;
	int	width;

	doc->map_start = -1;
	doc->map_height = 0;
	doc->map_width = 0;
	i = 0;
	while (i < doc->cub_count && !is_map_line(doc->cub[i]))
		i++;
	if (i == doc->cub_count)
		return (false);
	doc->map_start = i;
	while (i < doc->cub_count && is_map_line(doc->cub[i]))
	{
		width = (int)ft_strlen(doc->cub[i]);
		if (width > doc->map_width)
			doc->map_width = width;
		doc->map_height++;
		i++;
	}
	return (true);
}

void	make_sector_path(t_editor_doc *doc)
{
	char	*extension;

	ft_strlcpy(doc->sector_path, doc->cub_path, LINE_SIZE);
	extension = ft_strrchr(doc->sector_path, '.');
	if (extension && !ft_strcmp(extension, ".cub"))
		ft_strlcpy(extension, ".sectors", LINE_SIZE - (extension
				- doc->sector_path));
	else
		ft_strlcat(doc->sector_path, ".sectors", LINE_SIZE);
}

bool	append_line(char **lines, int *count, const char *text)
{
	if (*count >= DNK_MAX_LINES)
		return (false);
	lines[*count] = editor_dup(text);
	if (!lines[*count])
		return (false);
	(*count)++;
	return (true);
}

void	scan_sector_grid(t_editor_doc *doc)
{
	int	i;

	doc->grid_start = -1;
	i = 0;
	while (i < doc->sector_count)
	{
		if (!ft_strcmp(doc->sectors[i], "GRID"))
			doc->grid_start = i;
		i++;
	}
}
