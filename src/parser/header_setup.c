/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	**ft_values_setup(int *readval)
{
	int	i;

	i = 0;
	while (i < 9)
		readval[i++] = 0;
	return (NULL);
}

static void	clear_base_textures(t_header *header)
{
	ft_bzero(header->no, LINE_SIZE);
	ft_bzero(header->ea, LINE_SIZE);
	ft_bzero(header->so, LINE_SIZE);
	ft_bzero(header->we, LINE_SIZE);
	ft_bzero(header->door, LINE_SIZE);
	ft_bzero(header->floor_texture, LINE_SIZE);
	ft_bzero(header->ceiling_texture, LINE_SIZE);
	ft_bzero(header->sky_texture, LINE_SIZE);
	ft_bzero(header->sprite_texture, LINE_SIZE);
}

static void	clear_optional_textures(t_header *header)
{
	ft_bzero(header->transparent_texture, LINE_SIZE);
	ft_bzero(header->decal_texture, LINE_SIZE);
	ft_bzero(header->vending_machine_texture, LINE_SIZE);
	ft_bzero(header->laptop_texture, LINE_SIZE);
	ft_bzero(header->next_level, LINE_SIZE);
}

static void	clear_texture_arrays(t_header *header)
{
	int	i;

	i = SPRITE_FRAME_NB;
	while (i--)
		ft_bzero(header->sprite_frame_textures[i], LINE_SIZE);
	i = ENEMY_TYPES_NB;
	while (i--)
		ft_bzero(header->enemy_texture[i], LINE_SIZE);
	i = DECORATION_TYPES_NB;
	while (i--)
		ft_bzero(header->decoration_texture[i], LINE_SIZE);
}

void	ft_setup_header(t_header *header)
{
	header->floor[0] = -1;
	header->floor[1] = -1;
	header->floor[2] = -1;
	header->ceiling[0] = -1;
	header->ceiling[1] = -1;
	header->ceiling[2] = -1;
	clear_base_textures(header);
	clear_optional_textures(header);
	clear_texture_arrays(header);
}
