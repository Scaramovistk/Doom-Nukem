/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	set_grid_block(t_game *g, t_authored_action *a)
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

void	set_sector_property(t_game *g, t_authored_action *a)
{
	t_sector	*sector;

	if (a->target < 0 || a->target >= SECTOR_MAX
		|| !g->map.sectors[a->target].active)
		return ;
	sector = &g->map.sectors[a->target];
	if (a->type == AUTH_ACTION_FLOOR && a->values[0] <= sector->ceil_z - 0.25)
		sector->floor_z = a->values[0];
	else if (a->type == AUTH_ACTION_CEILING && a->values[0] >= sector->floor_z
		+ 0.25)
		sector->ceil_z = a->values[0];
	else if (a->type == AUTH_ACTION_LIGHT)
		sector->light = a->value;
}

void	set_object_property(t_game *g, t_authored_action *a)
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
		object->collision_radius = object->scale * WORLD_OBJECT_COLLISION_RATIO;
	}
	else if (a->type == AUTH_ACTION_OBJECT_TEXTURE)
		object->texture = a->value;
}

void	set_wall_property(t_game *g, t_authored_action *a)
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
