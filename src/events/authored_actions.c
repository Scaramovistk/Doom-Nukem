/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authored_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/05 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	block_from_name(const char *name)
{
	if (!ft_strcmp((char *)name, "EMPTY"))
		return (EMPTY);
	if (!ft_strcmp((char *)name, "WALL"))
		return (WALL);
	if (!ft_strcmp((char *)name, "DOOR"))
		return (DOOR);
	if (!ft_strcmp((char *)name, "GLASS"))
		return (TRANSPARENT_WALL);
	if (!ft_strcmp((char *)name, "DECAL"))
		return (DECAL_WALL);
	return (-1);
}

static bool	parse_sector_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "FLOOR"))
		a->type = AUTH_ACTION_FLOOR;
	else if (!ft_strcmp(kind, "CEILING"))
		a->type = AUTH_ACTION_CEILING;
	else if (!ft_strcmp(kind, "LIGHT"))
		a->type = AUTH_ACTION_LIGHT;
	else
		return (false);
	if (a->type == AUTH_ACTION_LIGHT)
		return (sscanf(args, "%d %d", &a->target, &a->value) == 2
			&& a->value >= 0 && a->value <= 255);
	return (sscanf(args, "%d %lf", &a->target, &a->values[0]) == 2);
}

static bool	parse_object_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "OBJECT_MOVE"))
	{
		a->type = AUTH_ACTION_OBJECT_MOVE;
		return (sscanf(args, "%d %lf %lf", &a->target, &a->values[0],
				&a->values[1]) == 3);
	}
	if (!ft_strcmp(kind, "OBJECT_BLOCK"))
	{
		a->type = AUTH_ACTION_OBJECT_BLOCK;
		return (sscanf(args, "%d %d", &a->target, &a->value) == 2
			&& (a->value == 0 || a->value == 1));
	}
	if (!ft_strcmp(kind, "OBJECT_SCALE"))
	{
		a->type = AUTH_ACTION_OBJECT_SCALE;
		return (sscanf(args, "%d %lf", &a->target, &a->values[0]) == 2
			&& a->values[0] > 0.0);
	}
	if (ft_strcmp(kind, "OBJECT_TEXTURE"))
		return (false);
	a->type = AUTH_ACTION_OBJECT_TEXTURE;
	return (sscanf(args, "%d %d", &a->target, &a->value) == 2
		&& a->value >= 0 && a->value < TEXTURES_NB);
}

static bool	parse_wall_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "WALL_MOVE"))
	{
		a->type = AUTH_ACTION_WALL_MOVE;
		return (sscanf(args, "%d %lf %lf %lf %lf", &a->target,
				&a->values[0], &a->values[1], &a->values[2],
				&a->values[3]) == 5);
	}
	if (ft_strcmp(kind, "WALL_TEXTURE"))
		return (false);
	a->type = AUTH_ACTION_WALL_TEXTURE;
	return (sscanf(args, "%d %d", &a->target, &a->value) == 2
		&& a->value >= 0 && a->value < TEXTURES_NB);
}

static bool	parse_authored_action(const char *line, t_authored_action *a)
{
	char	kind[24];
	char	block[16];
	int		offset;

	ft_bzero(a, sizeof(*a));
	offset = 0;
	if (sscanf(line, "ACTION %d %d %lf %23s %n", &a->trigger.x,
			&a->trigger.y, &a->delay, kind, &offset) != 4 || a->delay < 0.0)
		return (false);
	if (!ft_strcmp(kind, "BLOCK"))
	{
		a->type = AUTH_ACTION_BLOCK;
		if (sscanf(line + offset, "%d %d %15s", &a->cell.x,
				&a->cell.y, block) != 3)
			return (false);
		a->value = block_from_name(block);
		return (a->value >= 0);
	}
	if (!ft_strcmp(kind, "TEXTURE_SWAP"))
	{
		a->type = AUTH_ACTION_TEXTURE_SWAP;
		return (sscanf(line + offset, "%d %d", &a->target, &a->value) == 2
			&& a->target >= 0 && a->target < TEXTURES_NB
			&& a->value >= 0 && a->value < TEXTURES_NB);
	}
	if (parse_sector_action(a, kind, (char *)line + offset))
		return (true);
	if (parse_object_action(a, kind, (char *)line + offset))
		return (true);
	return (parse_wall_action(a, kind, (char *)line + offset));
}

bool	authored_action_line_valid(const char *line)
{
	t_authored_action	action;

	return (parse_authored_action(line, &action));
}

static bool	trigger_exists(t_game *g, t_coord trigger)
{
	int	i;

	i = 0;
	while (i < g->map.switch_count)
	{
		if (g->map.switches[i].x == trigger.x
			&& g->map.switches[i].y == trigger.y)
			return (true);
		i++;
	}
	return (false);
}

static bool	texture_available(t_game *g, int texture)
{
	if (texture < 0 || texture >= TEXTURES_NB)
		return (false);
	if (texture == SPRITE_T && g->assets.has_sprite_frames)
		return (true);
	return (g->assets.textures[texture].source != NULL);
}

static bool	action_target_valid(t_game *g, t_authored_action *a)
{
	if (!trigger_exists(g, a->trigger))
		return (false);
	if (a->type == AUTH_ACTION_BLOCK)
	{
		if (!is_in_bounds(a->cell, g))
			return (false);
		if (a->value == DOOR)
			return (texture_available(g, DOOR_T));
		if (a->value == TRANSPARENT_WALL)
			return (texture_available(g, TRANSPARENT_T));
		if (a->value == DECAL_WALL)
			return (texture_available(g, DECAL_T));
		return (true);
	}
	if (a->type >= AUTH_ACTION_FLOOR && a->type <= AUTH_ACTION_LIGHT)
		return (a->target >= 0 && a->target < SECTOR_MAX
			&& g->map.sectors[a->target].active);
	if (a->type == AUTH_ACTION_TEXTURE_SWAP)
		return (texture_available(g, a->target)
			&& texture_available(g, a->value));
	if (a->type >= AUTH_ACTION_OBJECT_MOVE
		&& a->type <= AUTH_ACTION_OBJECT_TEXTURE)
	{
		if (a->target < 0 || a->target >= g->map.object_count)
			return (false);
		return (a->type != AUTH_ACTION_OBJECT_TEXTURE
			|| texture_available(g, a->value));
	}
	if (a->type == AUTH_ACTION_WALL_MOVE
		|| a->type == AUTH_ACTION_WALL_TEXTURE)
	{
		if (a->target < 0 || a->target >= g->map.segment_count)
			return (false);
		return (a->type != AUTH_ACTION_WALL_TEXTURE
			|| texture_available(g, a->value));
	}
	return (true);
}

bool	add_authored_action(t_game *g, const char *line)
{
	t_authored_action	action;

	if (g->map.action_count >= AUTHORED_ACTION_MAX
		|| !parse_authored_action(line, &action)
		|| !action_target_valid(g, &action))
		return (false);
	g->map.actions[g->map.action_count++] = action;
	return (true);
}

static void	set_grid_block(t_game *g, t_authored_action *a)
{
	t_door	*door;

	if (!is_in_bounds(a->cell, g))
		return ;
	g->map.grid[a->cell.y][a->cell.x] = (t_block)a->value;
	if (a->value != DOOR)
		return ;
	door = &g->map.doors[a->cell.y][a->cell.x];
	door->opening_state = 0.0;
	door->is_opening = false;
	door->is_locked = false;
	door->discovered = true;
}

static void	set_sector_property(t_game *g, t_authored_action *a)
{
	t_sector	*sector;

	if (a->target < 0 || a->target >= SECTOR_MAX
		|| !g->map.sectors[a->target].active)
		return ;
	sector = &g->map.sectors[a->target];
	if (a->type == AUTH_ACTION_FLOOR
		&& a->values[0] <= sector->ceil_z - 0.25)
		sector->floor_z = a->values[0];
	else if (a->type == AUTH_ACTION_CEILING
		&& a->values[0] >= sector->floor_z + 0.25)
		sector->ceil_z = a->values[0];
	else if (a->type == AUTH_ACTION_LIGHT)
		sector->light = a->value;
}

static void	set_object_property(t_game *g, t_authored_action *a)
{
	t_world_object	*object;

	if (a->target < 0 || a->target >= g->map.object_count)
		return ;
	object = &g->map.objects[a->target];
	if (a->type == AUTH_ACTION_OBJECT_MOVE)
	{
		object->pos = (t_position){a->values[0], a->values[1]};
		g->map.sprites[object->sprite_index] = object->pos;
	}
	else if (a->type == AUTH_ACTION_OBJECT_BLOCK)
		object->blocks_passage = (a->value != 0);
	else if (a->type == AUTH_ACTION_OBJECT_SCALE)
	{
		object->scale = a->values[0];
		object->collision_radius = object->scale
			* WORLD_OBJECT_COLLISION_RATIO;
	}
	else if (a->type == AUTH_ACTION_OBJECT_TEXTURE)
		object->texture = a->value;
}

static void	set_wall_property(t_game *g, t_authored_action *a)
{
	t_wall_segment	*wall;

	if (a->target < 0 || a->target >= g->map.segment_count)
		return ;
	wall = &g->map.segments[a->target];
	if (a->type == AUTH_ACTION_WALL_TEXTURE)
		wall->texture = a->value;
	else
	{
		wall->a = (t_position){a->values[0], a->values[1]};
		wall->b = (t_position){a->values[2], a->values[3]};
	}
}

void	run_authored_action(t_game *g, int index)
{
	t_authored_action	*a;
	t_texture			tmp;

	if (index < 0 || index >= g->map.action_count)
		return ;
	a = &g->map.actions[index];
	if (a->type == AUTH_ACTION_BLOCK)
		set_grid_block(g, a);
	else if (a->type >= AUTH_ACTION_FLOOR && a->type <= AUTH_ACTION_LIGHT)
		set_sector_property(g, a);
	else if (a->type == AUTH_ACTION_TEXTURE_SWAP)
	{
		tmp = g->assets.textures[a->target];
		g->assets.textures[a->target] = g->assets.textures[a->value];
		g->assets.textures[a->value] = tmp;
	}
	else if (a->type >= AUTH_ACTION_OBJECT_MOVE
		&& a->type <= AUTH_ACTION_OBJECT_TEXTURE)
		set_object_property(g, a);
	else if (a->type == AUTH_ACTION_WALL_MOVE
		|| a->type == AUTH_ACTION_WALL_TEXTURE)
		set_wall_property(g, a);
}

bool	trigger_authored_actions(t_game *g, t_coord trigger)
{
	t_world_event	event;
	int				i;
	bool			queued;

	queued = false;
	i = 0;
	while (i < g->map.action_count)
	{
		if (g->map.actions[i].trigger.x == trigger.x
			&& g->map.actions[i].trigger.y == trigger.y)
		{
			ft_bzero(&event, sizeof(event));
			event.action = EVENT_AUTHORED_ACTION;
			event.timer = g->map.actions[i].delay;
			event.target = i;
			event.active = true;
			queue_world_event(g, event);
			queued = true;
		}
		i++;
	}
	if (queued)
	{
		show_message(g, "AUTHORED SEQUENCE ACTIVE", MESSAGE_DISPLAY_TIME);
		play_sound_effect(g, "switch");
	}
	return (queued);
}
