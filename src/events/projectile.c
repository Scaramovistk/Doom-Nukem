/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_item_sprite(t_game *g, int sprite_index)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active
			&& g->map.items[i].sprite_index == sprite_index)
			return (true);
		i++;
	}
	return (false);
}

static bool	is_vending_machine_sprite(t_game *g, int sprite_index)
{
	return (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == sprite_index);
}

static bool	is_laptop_sprite(t_game *g, int sprite_index)
{
	int	i;

	i = 0;
	while (i < g->map.laptop_count)
	{
		if (g->map.laptops[i++].sprite_index == sprite_index)
			return (true);
	}
	return (false);
}

static bool	is_decoration_sprite(t_game *g, int sprite_index)
{
	int	i;

	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == sprite_index)
			return (true);
		i++;
	}
	return (false);
}

static void	relink_moved_sprite(t_game *g, int old_index, int new_index)
{
	int	i;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active
			&& g->map.items[i].sprite_index == old_index)
			g->map.items[i].sprite_index = new_index;
		i++;
	}
	if (g->map.vending_machine.active
		&& g->map.vending_machine.sprite_index == old_index)
		g->map.vending_machine.sprite_index = new_index;
	if (g->map.has_flag && g->map.flag_sprite_index == old_index)
		g->map.flag_sprite_index = new_index;
	i = 0;
	while (i < g->map.laptop_count)
	{
		if (g->map.laptops[i].sprite_index == old_index)
			g->map.laptops[i].sprite_index = new_index;
		i++;
	}
	i = 0;
	while (i < g->map.decoration_count)
	{
		if (g->map.decorations[i].sprite_index == old_index)
			g->map.decorations[i].sprite_index = new_index;
		i++;
	}
	i = 0;
	while (i < g->map.object_count)
	{
		if (g->map.objects[i].sprite_index == old_index)
			g->map.objects[i].sprite_index = new_index;
		i++;
	}
}

static void	remove_sprite_target(t_game *g, int index, int damage)
{
	int	last;

	if (index < 0 || index >= g->map.sprite_count || is_item_sprite(g, index)
		|| is_vending_machine_sprite(g, index) || is_laptop_sprite(g, index)
		|| is_decoration_sprite(g, index))
		return ;
	last = g->map.sprite_count - 1;
	if (index != last)
	{
		g->map.sprites[index] = g->map.sprites[last];
		relink_moved_sprite(g, last, index);
	}
	g->map.sprite_count--;
	g->hud.score += damage;
}

bool	hit_sprite(t_game *g, t_projectile *p, t_position pos)
{
	double	dx;
	double	dy;
	int		i;

	if (p->from_enemy)
		return (false);
	i = 0;
	while (i < g->map.sprite_count)
	{
		if (is_decoration_sprite(g, i)
			|| is_vending_machine_sprite(g, i)
			|| is_laptop_sprite(g, i))
		{
			i++;
			continue ;
		}
		dx = g->map.sprites[i].x - pos.x;
		dy = g->map.sprites[i].y - pos.y;
		if (dx * dx + dy * dy <= PROJECTILE_HIT_RADIUS
			* PROJECTILE_HIT_RADIUS)
		{
			if (damage_enemy_at_sprite(g, i, p->damage))
				return (true);
			remove_sprite_target(g, i, p->damage);
			return (true);
		}
		i++;
	}
	return (false);
}

static bool	projectile_cell_is_blocked(t_game *g, t_position pos, double z)
{
	t_coord	cell;
	t_block	block;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (!is_in_bounds(cell, g))
		return (true);
	block = g->map.grid[cell.y][cell.x];
	if (block == WALL || block == DECAL_WALL || block == TRANSPARENT_WALL)
		return (true);
	if (block == DOOR && !is_door_open(cell, g->map.doors))
		return (true);
	return (z <= get_floor_z_at(g, pos) + 0.01
		|| z >= get_ceiling_z_at(g, pos) - 0.01);
}

bool	projectile_path_is_blocked(t_game *g, t_position start,
		t_position end, double z)
{
	t_position	pos;
	double		dx;
	double		dy;
	double		length;
	double		progress;

	dx = end.x - start.x;
	dy = end.y - start.y;
	length = sqrt(dx * dx + dy * dy);
	if (length < 0.001)
		return (projectile_cell_is_blocked(g, end, z));
	progress = 0.0;
	while (progress <= length)
	{
		pos.x = start.x + dx * progress / length;
		pos.y = start.y + dy * progress / length;
		if (projectile_cell_is_blocked(g, pos, z))
			return (true);
		progress += 0.05;
	}
	return (projectile_cell_is_blocked(g, end, z));
}

bool	hit_wall(t_game *g, t_projectile *p, t_position pos)
{
	t_coord	cell;

	if (!projectile_path_is_blocked(g, p->pos, pos, p->z))
		return (false);
	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (is_in_bounds(cell, g) && g->map.grid[cell.y][cell.x] == WALL)
		g->map.grid[cell.y][cell.x] = DECAL_WALL;
	return (true);
}
