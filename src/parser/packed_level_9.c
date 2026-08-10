/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_level_9.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	pack_source_valid(char *src)
{
	t_game	g;
	char	*argv[2];
	bool	valid;

	init_game_struct(&g);
	argv[0] = "doom-nukem";
	argv[1] = src;
	valid = ft_parse_file(2, argv, &g);
	free_all(&g);
	if (!valid)
		ft_parsing_error("Refusing to pack an invalid level.", 0);
	return (valid);
}

int	pack_level_file(char *src, char *dst)
{
	FILE	*out;
	char	*lines[DNK_MAX_LINES];
	int		count;

	count = 0;
	if (!pack_source_valid(src))
		return (0);
	if (!load_cub_text(src, lines, &count))
		return (ft_parsing_error("Unable to read source level.", 0));
	out = fopen(dst, "w");
	if (!out)
		return (ft_parsing_error("Unable to write packed level.", 0));
	fprintf(out, "%s\n", DNK_MAGIC);
	if (!write_all_assets(out, src))
		return (fclose(out), unlink(dst), free_cub_lines(lines, count),
			ft_parsing_error("Unable to embed every level asset.", 0));
	write_cub_section(out, lines, count);
	if (!write_sector_sidecar(out, src))
		write_default_sector_grid(out, lines, count);
	fclose(out);
	free_cub_lines(lines, count);
	return (1);
}
