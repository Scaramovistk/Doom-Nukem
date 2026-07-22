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
	else if (c == '2')
		return (DOOR);
	else if (c == '3')
		return (SPRITE);
	else if (c == '4')
		return (TRANSPARENT_WALL);
	else if (c == '5' || c == 'T')
		return (DECAL_WALL);
	else if ((c >= '6' && c <= '9') || c == 'H' || c == 'M' || c == 'X')
		return (EMPTY);
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

static bool	ft_is_item_char(char c)
{
	return (c >= '6' && c <= '9');
}

static int	count_items(char **map, int lines, int width)
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
			if (ft_is_item_char(map[vert][hor]))
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

static int	item_default_quantity(int type)
{
	static const int	amounts[ITEM_TYPES_NB] = {25, 10, 1, 5};

	if (type < 0 || type >= ITEM_TYPES_NB)
		return (1);
	return (amounts[type]);
}

static bool	item_default_blocks(int type)
{
	(void)type;
	return (false);
}

static void	add_items(char **map, int lines, int width, t_game *g, int deco)
{
	int		vert;
	int		hor;
	int		i;
	int		type;

	if (!g->map.item_count)
		return ;
	g->map.items = calloc_s(g->map.item_count, sizeof(t_item), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
		{
			if (!ft_is_item_char(map[vert][hor]))
				continue ;
			type = map[vert][hor] - '6';
			g->map.items[i].pos = (t_position){hor + 0.5, vert + 0.5};
			g->map.items[i].type = type;
			g->map.items[i].quantity = item_default_quantity(type);
			g->map.items[i].blocks_passage = item_default_blocks(type);
			g->map.items[i].active = true;
			g->map.items[i].sprite_index = deco + i;
			g->map.sprites[deco + i] = g->map.items[i].pos;
			i++;
		}
	}
}

static void	add_enemies(int count, t_game *g)
{
	int	i;

	g->map.enemy_count = count;
	if (!count)
		return ;
	g->map.enemies = calloc_s(count, sizeof(t_enemy), g);
	i = 0;
	while (i < count)
	{
		g->map.enemies[i].pos = g->map.sprites[i];
		g->map.enemies[i].health = ENEMY_HEALTH;
		g->map.enemies[i].sprite_index = i;
		g->map.enemies[i].attack_timer = 0.0;
		g->map.enemies[i].active = true;
		i++;
	}
}

static void	add_sprites(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;
	int	deco;

	deco = count_sprites(map, lines, width);
	g->map.item_count = count_items(map, lines, width);
	g->map.sprite_count = deco + g->map.item_count;
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
	add_enemies(deco, g);
	add_items(map, lines, width, g, deco);
}

static int	count_char(char **map, int lines, int width, char target)
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
			if (map[vert][hor] == target)
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

static void	add_switches(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.switch_count = count_char(map, lines, width, 'T');
	if (!g->map.switch_count)
		return ;
	g->map.switches = calloc_s(g->map.switch_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'T')
				g->map.switches[i++] = (t_coord){hor, vert};
	}
}

static void	add_hazards(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.hazard_count = count_char(map, lines, width, 'H');
	if (!g->map.hazard_count)
		return ;
	g->map.hazard_zones = calloc_s(g->map.hazard_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'H')
				g->map.hazard_zones[i++] = (t_coord){hor, vert};
	}
}

static void	add_messages(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.message_count = count_char(map, lines, width, 'M');
	if (!g->map.message_count)
		return ;
	g->map.message_zones = calloc_s(g->map.message_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'M')
				g->map.message_zones[i++] = (t_coord){hor, vert};
	}
}

static void	add_exits(char **map, int lines, int width, t_game *g)
{
	int	vert;
	int	hor;
	int	i;

	g->map.exit_count = count_char(map, lines, width, 'X');
	if (!g->map.exit_count)
		return ;
	g->map.exit_zones = calloc_s(g->map.exit_count, sizeof(t_coord), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
			if (map[vert][hor] == 'X')
				g->map.exit_zones[i++] = (t_coord){hor, vert};
	}
}

static void	add_interactables(char **map, int lines, int width, t_game *g)
{
	add_switches(map, lines, width, g);
	add_hazards(map, lines, width, g);
	add_messages(map, lines, width, g);
	add_exits(map, lines, width, g);
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
	add_interactables(map, lines - vals[2], width, g);
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
	init_default_sectors(g);
	ft_free_map(map, lines);
}

void	ft_populate_info(t_header *h, t_game *g)
{
	int	ceiling;
	int	floor;
	int	i;

	g->assets.textures[NORTH].source = s_alloc(ft_strdup(h->no), g);
	g->assets.textures[SOUTH].source = s_alloc(ft_strdup(h->so), g);
	g->assets.textures[WEST].source = s_alloc(ft_strdup(h->we), g);
	g->assets.textures[EAST].source = s_alloc(ft_strdup(h->ea), g);
	if (h->door[0])
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
	if (h->transparent_texture[0])
		g->assets.textures[TRANSPARENT_T].source = s_alloc(
				ft_strdup(h->transparent_texture), g);
	if (h->decal_texture[0])
		g->assets.textures[DECAL_T].source = s_alloc(
				ft_strdup(h->decal_texture), g);
	i = 0;
	while (i < SPRITE_FRAME_NB && h->sprite_frame_textures[i][0])
	{
		g->assets.sprite_frames[i].source = s_alloc(
				ft_strdup(h->sprite_frame_textures[i]), g);
		i++;
	}
	if (i == SPRITE_FRAME_NB)
		g->assets.has_sprite_frames = true;
}
