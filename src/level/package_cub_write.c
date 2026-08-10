/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   package_cub_write.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	clear_packed_fallbacks(t_game *g)
{
	int	i;
	int	j;

	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
			g->assets.hud_weapons[i][j++].source = NULL;
		i++;
	}
	g->assets.ammo_icon.source = NULL;
	i = 0;
	while (i < ITEM_TYPES_NB)
		g->assets.item_icons[i++].source = NULL;
	g->audio.sound_dir[0] = '\0';
}

const char	*find_asset_path(t_dnk *dnk, const char *key)
{
	int	i;

	i = 0;
	while (i < dnk->asset_count)
	{
		if (ft_strcmp((char *)key, dnk->assets[i].key) == 0)
			return (dnk->assets[i].path);
		i++;
	}
	return (NULL);
}

void	append_replaced(char *dst, const char *src, t_dnk *dnk)
{
	int			i;

	while (*src && ft_strlen(dst) < LINE_SIZE - 1)
	{
		if (*src != '@')
		{
			i = ft_strlen(dst);
			dst[i] = *src++;
			dst[i + 1] = '\0';
			continue ;
		}
		append_asset_reference(dst, &src, dnk);
	}
}

bool	write_temp_cub(t_dnk *dnk)
{
	int		fd;
	int		i;
	char	line[LINE_SIZE];

	fd = open(dnk->cub_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (false);
	i = 0;
	while (i < dnk->cub_count)
	{
		line[0] = '\0';
		append_replaced(line, dnk->cub_lines[i], dnk);
		if (write(fd, line, ft_strlen(line)) < 0 || write(fd, "\n", 1) != 1)
			return (close(fd), false);
		i++;
	}
	close(fd);
	return (true);
}

int	sector_id_from_char(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	return (0);
}
