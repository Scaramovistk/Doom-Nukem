/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_level.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/22 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	trim_eol(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	while (len && (line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
}

bool	starts_with(const char *text, const char *prefix)
{
	return (ft_strncmp(text, prefix, ft_strlen(prefix)) == 0);
}

void	append_path(char *dst, const char *part)
{
	if (ft_strlcat(dst, part, LINE_SIZE) >= LINE_SIZE)
	{
		perror("Packed level path is too long");
		exit(EXIT_FAILURE);
	}
}

void	make_unpack_dir(t_dnk *dnk, t_game *g)
{
	char	sub_dir[LINE_SIZE];

	mkdir("build", 0775);
	snprintf(dnk->dir, LINE_SIZE, "%s_%d", DNK_UNPACK_ROOT, getpid());
	mkdir(dnk->dir, 0775);
	ft_strlcpy(sub_dir, dnk->dir, LINE_SIZE);
	append_path(sub_dir, "/sounds");
	mkdir(sub_dir, 0775);
	ft_strlcpy(sub_dir, dnk->dir, LINE_SIZE);
	append_path(sub_dir, "/hud");
	mkdir(sub_dir, 0775);
	ft_strlcpy(dnk->cub_path, dnk->dir, LINE_SIZE);
	append_path(dnk->cub_path, "/level.cub");
	ft_strlcpy(g->unpack_dir, dnk->dir, LINE_SIZE);
	g->unpacked_level = true;
}

int	hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}
