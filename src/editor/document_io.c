/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   document_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	default_sectors(t_editor_doc *doc)
{
	char	*row;
	int		y;

	if (!append_line(doc->sectors, &doc->sector_count,
			"SECTOR 0 0.00 1.00 0.00 0.00 255") || !append_line(doc->sectors,
			&doc->sector_count, "GRID"))
		return (false);
	row = malloc(doc->map_width + 1);
	if (!row)
		return (false);
	ft_memset(row, '0', doc->map_width);
	row[doc->map_width] = '\0';
	y = 0;
	while (y++ < doc->map_height)
		if (!append_line(doc->sectors, &doc->sector_count, row))
			return (free(row), false);
	free(row);
	scan_sector_grid(doc);
	return (true);
}

bool	load_document(t_editor_doc *doc, char *src, char *output)
{
	ft_bzero(doc, sizeof(*doc));
	ft_strlcpy(doc->cub_path, src, LINE_SIZE);
	ft_strlcpy(doc->output_path, output, LINE_SIZE);
	if (!read_lines(doc->cub_path, doc->cub, &doc->cub_count) || !scan_map(doc))
		return (free_document(doc), false);
	make_sector_path(doc);
	read_lines(doc->sector_path, doc->sectors, &doc->sector_count);
	if (!doc->sector_count && !default_sectors(doc))
		return (free_document(doc), false);
	scan_sector_grid(doc);
	if (doc->grid_start < 0)
		return (free_document(doc), false);
	return (true);
}

bool	editor_open(t_editor_doc *doc, char *src, char *output)
{
	return (load_document(doc, src, output));
}

bool	write_lines(const char *path, char **lines, int count)
{
	char	tmp[LINE_SIZE];
	FILE	*out;
	int		i;

	if (snprintf(tmp, sizeof(tmp), "%s.tmp.%d", path,
			getpid()) >= (int) sizeof(tmp))
		return (false);
	out = fopen(tmp, "w");
	if (!out)
		return (false);
	i = 0;
	while (i < count)
	{
		if (fprintf(out, "%s\n", lines[i++]) < 0)
			return (fclose(out), unlink(tmp), false);
	}
	if (fclose(out) != 0 || rename(tmp, path) != 0)
		return (unlink(tmp), false);
	return (true);
}

bool	save_document(t_editor_doc *doc)
{
	if (!editor_border_valid(doc))
		return (printf("Map border must contain only solid walls ('1').\n"),
			false);
	if (!write_lines(doc->cub_path, doc->cub, doc->cub_count)
		|| !write_lines(doc->sector_path, doc->sectors, doc->sector_count))
		return (printf("Save failed.\n"), false);
	doc->dirty = false;
	printf("Saved %s and %s\n", doc->cub_path, doc->sector_path);
	return (true);
}
