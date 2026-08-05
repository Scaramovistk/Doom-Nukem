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
	else if (c == '3' || c == 'K' || c == 'I' || c == 'D' || c == 'C')
		return (SPRITE);
	else if (c == '4')
		return (TRANSPARENT_WALL);
	else if (c == '5' || c == 'T')
		return (DECAL_WALL);
	else if (c >= 'g' && c <= 'l')
		return (DECAL_WALL);
	else if ((c >= '6' && c <= '9') || c == 'H' || c == 'M' || c == 'X'
		|| c == 'L' || c == 'P' || c == 'V' || c == 'G')
		return (EMPTY);
	else
		return (NULL_BLOCK);
}

static bool	ft_is_enemy_char(char c)
{
	return (c == '3' || c == 'K' || c == 'I' || c == 'D' || c == 'C');
}

static int	count_decorations(char **map, int lines, int width)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
			count += (map[y][x++] >= 'g' && map[y][x - 1] <= 'l');
		y++;
	}
	return (count);
}

static void	add_decorations(char **map, int lines, int width, t_game *g)
{
	int	x;
	int	y;
	int	i;

	g->map.decoration_count = count_decorations(map, lines, width);
	if (!g->map.decoration_count)
		return ;
	g->map.decorations = calloc_s(g->map.decoration_count,
			sizeof(t_decoration), g);
	i = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
		{
			if (map[y][x] >= 'g' && map[y][x] <= 'l')
			{
				g->map.decorations[i].pos = (t_position){x + 0.5, y + 0.5};
				g->map.decorations[i++].type = map[y][x] - 'g';
			}
			x++;
		}
		y++;
	}
}

static int	count_vending_machines(char **map, int lines, int width)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
			count += (map[y][x++] == 'V');
		y++;
	}
	return (count);
}

static bool	add_vending_machine(char **map, int lines, int width, t_game *g,
		int index)
{
	int	x;
	int	y;

	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
		{
			if (map[y][x] == 'V')
			{
				g->map.vending_machine.pos = (t_position){x + 0.5, y + 0.5};
				g->map.vending_machine.sprite_index = index;
				g->map.vending_machine.active = true;
				g->map.sprites[index] = g->map.vending_machine.pos;
				return (true);
			}
			x++;
		}
		y++;
	}
	return (false);
}

static int	enemy_type_from_char(char c)
{
	if (c == 'K')
		return (1);
	if (c == 'I')
		return (2);
	if (c == 'D')
		return (3);
	if (c == 'C')
		return (4);
	return (0);
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
			if (ft_is_enemy_char(map[vert][hor]))
				count++;
			hor++;
		}
		vert++;
	}
	return (count);
}

static int	count_flags(char **map, int lines, int width)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	y = 0;
	while (y < lines)
	{
		x = 0;
		while (x < width)
			count += (map[y][x++] == 'G');
		y++;
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

typedef struct s_enemy_stats
{
	int		health;
	bool	is_ranged;
	double	move_speed;
	int		contact_damage;
	double	attack_delay;
	double	attack_range;
	double	alert_range;
	double	fire_delay;
	double	ranged_range;
	int		projectile_damage;
	int		score_value;
}				t_enemy_stats;

static const t_enemy_stats	*enemy_stats_for_type(int type)
{
	static const t_enemy_stats	table[5] = {
	{20, false, ENEMY_MOVE_SPEED, 6, ENEMY_ATTACK_DELAY,
		ENEMY_ATTACK_RANGE, ENEMY_ALERT_RANGE, 0.0, 0.0, 0, 10},
	{30, true, ENEMY_MOVE_SPEED, 0, 0.0,
		0.0, ENEMY_ALERT_RANGE, ENEMY_FIRE_DELAY, ENEMY_RANGED_RANGE, 6, 25},
	{45, true, ENEMY_MOVE_SPEED, 0, 0.0,
		0.0, ENEMY_ALERT_RANGE, 1.3, ENEMY_RANGED_RANGE, 8, 35},
	{60, false, ENEMY_MOVE_SPEED * 1.6, 12, 0.6,
		ENEMY_ATTACK_RANGE, ENEMY_ALERT_RANGE * 1.3, 0.0, 0.0, 0, 45},
	{120, true, ENEMY_MOVE_SPEED * 0.5, 0, 0.0,
		0.0, ENEMY_ALERT_RANGE, 1.8, ENEMY_RANGED_RANGE * 1.2, 12, 60},
	};

	if (type < 0 || type >= 5)
		type = 0;
	return (&table[type]);
}

static void	add_enemies(int count, int *types, t_game *g)
{
	int						i;
	const t_enemy_stats	*stats;

	g->map.enemy_count = count;
	if (!count)
		return ;
	g->map.enemies = calloc_s(count, sizeof(t_enemy), g);
	i = 0;
	while (i < count)
	{
		stats = enemy_stats_for_type(types[i]);
		g->map.enemies[i].pos = g->map.sprites[i];
		g->map.enemies[i].health = stats->health;
		g->map.enemies[i].max_health = stats->health;
		g->map.enemies[i].sprite_index = i;
		g->map.enemies[i].attack_timer = 0.0;
		g->map.enemies[i].fire_timer = 0.0;
		g->map.enemies[i].type = types[i];
		g->map.enemies[i].is_ranged = stats->is_ranged;
		g->map.enemies[i].active = true;
		g->map.enemies[i].move_speed = stats->move_speed;
		g->map.enemies[i].contact_damage = stats->contact_damage;
		g->map.enemies[i].attack_delay = stats->attack_delay;
		g->map.enemies[i].attack_range_sq = stats->attack_range
			* stats->attack_range;
		g->map.enemies[i].alert_range_sq = stats->alert_range
			* stats->alert_range;
		g->map.enemies[i].fire_delay = stats->fire_delay;
		g->map.enemies[i].ranged_range_sq = stats->ranged_range
			* stats->ranged_range;
		g->map.enemies[i].projectile_damage = stats->projectile_damage;
		g->map.enemies[i].score_value = stats->score_value;
		i++;
	}
}

static void	add_sprites(char **map, int lines, int width, t_game *g)
{
	int		vert;
	int		hor;
	int		i;
	int		deco;
	int		vending;
	int		flags;
	int		*types;

	deco = count_sprites(map, lines, width);
	vending = count_vending_machines(map, lines, width);
	flags = count_flags(map, lines, width);
	if (vending > 1)
		error("Only one vending machine is allowed per map", g);
	g->map.item_count = count_items(map, lines, width);
	add_decorations(map, lines, width, g);
	g->map.sprite_count = deco + vending + flags + g->map.item_count;
	if (!g->map.sprite_count)
		return ;
	g->map.sprites = calloc_s(g->map.sprite_count, sizeof(t_position), g);
	types = NULL;
	if (deco)
		types = calloc_s(deco, sizeof(int), g);
	i = 0;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
		{
			if (ft_is_enemy_char(map[vert][hor]))
			{
				types[i] = enemy_type_from_char(map[vert][hor]);
				g->map.sprites[i++] = (t_position){hor + 0.5, vert + 0.5};
			}
		}
	}
	add_enemies(deco, types, g);
	if (vending)
		add_vending_machine(map, lines, width, g, deco);
	i = deco + vending;
	vert = -1;
	while (++vert < lines)
	{
		hor = -1;
		while (++hor < width)
		{
			if (map[vert][hor] == 'G')
			{
				g->map.has_flag = true;
				g->map.flag_pos = (t_position){hor + 0.5, vert + 0.5};
				g->map.flag_sprite_index = i++;
				g->map.sprites[g->map.flag_sprite_index] = g->map.flag_pos;
			}
		}
	}
	add_items(map, lines, width, g, deco + vending + flags);
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
	add_elevators(map, lines, width, g);
	add_secrets(map, lines, width, g);
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
	if (h->vending_machine_texture[0])
		g->assets.vending_machine.source = s_alloc(
				ft_strdup(h->vending_machine_texture), g);
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
	i = 0;
	while (i < ENEMY_TYPES_NB)
	{
		if (h->enemy_texture[i][0])
			g->assets.enemy_icons[i].source = s_alloc(
					ft_strdup(h->enemy_texture[i]), g);
		i++;
	}
	i = 0;
	while (i < DECORATION_TYPES_NB)
	{
		if (h->decoration_texture[i][0])
			g->assets.decoration_icons[i].source = s_alloc(
				ft_strdup(h->decoration_texture[i]), g);
		i++;
	}
	if (h->next_level[0])
		ft_strlcpy(g->level.next_level, h->next_level, LINE_SIZE);
}
