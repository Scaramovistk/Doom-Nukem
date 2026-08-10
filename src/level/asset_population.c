/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_population.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	populate_wall_assets(t_header *h, t_game *g)
{
	g->assets.textures[NORTH].source = s_alloc(ft_strdup(h->no), g);
	g->assets.textures[SOUTH].source = s_alloc(ft_strdup(h->so), g);
	g->assets.textures[WEST].source = s_alloc(ft_strdup(h->we), g);
	g->assets.textures[EAST].source = s_alloc(ft_strdup(h->ea), g);
	if (h->door[0])
		g->assets.textures[DOOR_T].source = s_alloc(ft_strdup(h->door), g);
}

void	populate_plane_assets(t_header *h, t_game *g)
{
	if (h->floor_texture[0])
		g->assets.floor_texture.source
			= s_alloc(ft_strdup(h->floor_texture), g);
	else
		g->assets.floor_color = (h->floor[0] << 16) | (h->floor[1] << 8)
			| h->floor[2];
	if (h->ceiling_texture[0])
		g->assets.ceiling_texture.source
			= s_alloc(ft_strdup(h->ceiling_texture), g);
	else
		g->assets.ceiling_color = (h->ceiling[0] << 16) | (h->ceiling[1] << 8)
			| h->ceiling[2];
	if (h->sky_texture[0])
	{
		g->assets.sky_texture.source = s_alloc(ft_strdup(h->sky_texture), g);
		g->assets.has_sky = true;
	}
}

void	populate_special_assets(t_header *h, t_game *g)
{
	if (h->sprite_texture[0])
		g->assets.textures[SPRITE_T].source
			= s_alloc(ft_strdup(h->sprite_texture), g);
	if (h->vending_machine_texture[0])
		g->assets.vending_machine.source
			= s_alloc(ft_strdup(h->vending_machine_texture), g);
	if (h->laptop_texture[0])
		g->assets.laptop.source = s_alloc(ft_strdup(h->laptop_texture), g);
	if (h->transparent_texture[0])
		g->assets.textures[TRANSPARENT_T].source
			= s_alloc(ft_strdup(h->transparent_texture), g);
	if (h->decal_texture[0])
		g->assets.textures[DECAL_T].source
			= s_alloc(ft_strdup(h->decal_texture), g);
}

void	populate_sprite_frames(t_header *h, t_game *g)
{
	int	i;

	i = 0;
	while (i < SPRITE_FRAME_NB && h->sprite_frame_textures[i][0])
	{
		g->assets.sprite_frames[i].source
			= s_alloc(ft_strdup(h->sprite_frame_textures[i]), g);
		i++;
	}
	if (i == SPRITE_FRAME_NB)
		g->assets.has_sprite_frames = true;
}

void	populate_entity_assets(t_header *h, t_game *g)
{
	int	i;

	i = 0;
	while (i < ENEMY_TYPES_NB)
	{
		if (h->enemy_texture[i][0])
			g->assets.enemy_icons[i].source
				= s_alloc(ft_strdup(h->enemy_texture[i]), g);
		i++;
	}
	i = 0;
	while (i < DECORATION_TYPES_NB)
	{
		if (h->decoration_texture[i][0])
			g->assets.decoration_icons[i].source
				= s_alloc(ft_strdup(h->decoration_texture[i]), g);
		i++;
	}
}
