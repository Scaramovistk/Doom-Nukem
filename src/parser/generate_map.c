/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:00 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:11:27 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_block	ft_convert_tblock(char c)
{
	if (c == '0')
		return (EMPTY);
	else if (c == '1')
		return (WALL);
	else if (c == 'S' || c == 'W' || c == 'N' || c == 'E')
		return (PLAYER);
	else if (c == '2' && MODE_BONUS)
		return (DOOR);
	else if (c == '2' && MODE_BONUS == 0)
		return (EMPTY);
	else if (c == '3')
		return (SPRITE);
	else
		return (NULL_BLOCK);
}

static int	count_sprites(char **map, int lines, int width)
{
	int	count;
	int	vert;
	int	hor;

	count = 0;
	vert = 0;
	while (vert < lines)
	{
		hor = 0;
		while (hor < width)
		{
			if (map[vert][hor] == '3')
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

static void	add_sprites(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.sprite_count = count_sprites(map, lines, width);
	if (!g->map.sprite_count)
		return ;
	g->map.sprites = calloc_s(g->map.sprite_count, sizeof(t_position), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
		{
			if (map[vert][hor] == '3')
				g->map.sprites[i++] = (t_position){hor + 0.5, vert + 0.5};
		}
	}
}

void	ft_populate_map(char **map, int *vals, t_game *g)
{
	int	lines;
	int	vert;
	int	hor;
	int	width;

	lines = vals[0];
	width = vals[1];
	add_sprites(map, lines - vals[2], width, g);
	g->map.grid = calloc_s(lines, sizeof(t_block *), g);
	vert = 0;
	while (vert < lines - vals[2])
	{
		hor = -1;
		g->map.grid[vert] = calloc_s(width, sizeof(t_block), g);
		while (++hor < width)
		{
			g->map.grid[vert][hor] = ft_convert_tblock(map[vert][hor]);
			if (g->map.grid[vert][hor] == SPRITE)
				g->map.grid[vert][hor] = EMPTY;
		}
		vert++;
	}
	g->map.height = lines - vals[2];
	g->map.width = width;
	ft_free_map(map, lines);
}

void	ft_populate_info(t_header *h, t_game *g)
{
	int	ceiling;
	int	floor;

	g->assets.textures[NORTH].source = s_alloc(ft_strdup(h->no), g);
	g->assets.textures[SOUTH].source = s_alloc(ft_strdup(h->so), g);
	g->assets.textures[WEST].source = s_alloc(ft_strdup(h->we), g);
	g->assets.textures[EAST].source = s_alloc(ft_strdup(h->ea), g);
	if (MODE_BONUS)
		g->assets.textures[DOOR_T].source = s_alloc(ft_strdup(h->door), g);
	if (h->floor_texture[0])
		g->assets.floor_texture.source = s_alloc(ft_strdup(h->floor_texture),
				g);
	else
	{
		floor = ((h->floor[0] << 16) | (h->floor[1] << 8) | h->floor[2]);
		g->assets.floor_color = floor;
	}
	if (h->ceiling_texture[0])
		g->assets.ceiling_texture.source = s_alloc(
				ft_strdup(h->ceiling_texture), g);
	else
	{
		ceiling = ((h->ceiling[0] << 16) | (h->ceiling[1] << 8)
				| h->ceiling[2]);
		g->assets.ceiling_color = ceiling;
	}
	if (h->sky_texture[0])
	{
		g->assets.sky_texture.source = s_alloc(ft_strdup(h->sky_texture), g);
		g->assets.has_sky = true;
	}
	if (h->sprite_texture[0])
		g->assets.textures[SPRITE_T].source = s_alloc(
				ft_strdup(h->sprite_texture), g);
}
