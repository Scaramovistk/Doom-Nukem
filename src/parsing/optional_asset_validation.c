/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optional_asset_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	good_optional_xpm(char *path, int *ok)
{
	int	fd;

	if (!path[0])
		return (1);
	if (!ft_xpm_extension(path))
		return (*ok = ft_parsing_error(XPMFILE, 0), 0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (*ok = ft_parsing_error(NOXPMS, 0), 0);
	close(fd);
	if (!ft_is_file(path))
		return (*ok = ft_parsing_error(DIRXPMS, 0), 0);
	return (1);
}

static int	good_sprite_frames(t_header *h, int *ok)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (++i < SPRITE_FRAME_NB)
		count += (h->sprite_frame_textures[i][0] != '\0');
	if (count != 0 && count != SPRITE_FRAME_NB)
		return (*ok = ft_parsing_error(PATHS, 0), 0);
	i = -1;
	while (++i < SPRITE_FRAME_NB)
		if (!good_optional_xpm(h->sprite_frame_textures[i], ok))
			return (0);
	return (1);
}

static int	good_enemy_textures(t_header *h, int *ok)
{
	int	i;

	i = -1;
	while (++i < ENEMY_TYPES_NB)
		if (!good_optional_xpm(h->enemy_texture[i], ok))
			return (0);
	return (1);
}

static int	good_next_level(t_header *h, int *ok)
{
	if (!h->next_level[0])
		return (1);
	if (!ft_cub_extension(h->next_level) && !ft_dnk_extension(h->next_level))
		return (*ok = ft_parsing_error(PATHS, 0), 0);
	return (1);
}

int	ft_good_optional_assets(t_header *h, int *ok)
{
	if (!good_optional_xpm(h->floor_texture, ok)
		|| !good_optional_xpm(h->ceiling_texture, ok)
		|| !good_optional_xpm(h->sky_texture, ok)
		|| !good_optional_xpm(h->sprite_texture, ok)
		|| !good_optional_xpm(h->vending_machine_texture, ok)
		|| !good_optional_xpm(h->laptop_texture, ok)
		|| !good_optional_xpm(h->transparent_texture, ok)
		|| !good_optional_xpm(h->decal_texture, ok))
		return (0);
	return (good_sprite_frames(h, ok) && good_enemy_textures(h, ok)
		&& good_next_level(h, ok));
}
