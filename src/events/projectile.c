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
}

static void	remove_sprite_target(t_game *g, int index, int damage)
{
	int	last;

	if (index < 0 || index >= g->map.sprite_count || is_item_sprite(g, index))
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

static bool	hit_sprite(t_game *g, t_projectile *p, t_position pos)
{
	double	dx;
	double	dy;
	int		i;

	i = 0;
	while (i < g->map.sprite_count)
	{
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

static bool	hit_wall(t_game *g, t_position pos)
{
	t_coord	cell;
	t_block	block;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (!is_in_bounds(cell, g))
		return (true);
	block = g->map.grid[cell.y][cell.x];
	if (block == WALL || block == DECAL_WALL || block == TRANSPARENT_WALL)
	{
		if (block == WALL)
			g->map.grid[cell.y][cell.x] = DECAL_WALL;
		return (true);
	}
	if (block == DOOR && !is_door_open(cell, g->map.doors))
		return (true);
	return (false);
}

static int	weapon_ammo_cost(t_game *g)
{
	if (g->hud.selected_weapon == 1)
		return (3);
	return (1);
}

static void	setup_projectile_weapon(t_projectile *p, t_game *g)
{
	if (g->hud.selected_weapon == 1)
	{
		p->velocity.x *= 0.75;
		p->velocity.y *= 0.75;
		p->damage = 25;
		p->size = PROJECTILE_SIZE + 4;
		p->color = ORANGE;
	}
	else
	{
		p->damage = 10;
		p->size = PROJECTILE_SIZE;
		p->color = YELLOW;
	}
}

void	fire_projectile(t_game *g)
{
	t_projectile	*p;
	int				i;
	int				ammo_cost;

	ammo_cost = weapon_ammo_cost(g);
	if (g->hud.ammo < ammo_cost)
	{
		show_message(g, "NO AMMO", MESSAGE_DISPLAY_TIME);
		return ;
	}
	i = 0;
	while (i < PROJECTILE_MAX && g->projectiles[i].active)
		i++;
	if (i == PROJECTILE_MAX)
		return ;
	p = &g->projectiles[i];
	p->pos = g->player.pos;
	p->pos.x += cos(g->player.orientation) * 0.35;
	p->pos.y += sin(g->player.orientation) * 0.35;
	p->velocity.x = cos(g->player.orientation) * PROJECTILE_SPEED;
	p->velocity.y = sin(g->player.orientation) * PROJECTILE_SPEED;
	setup_projectile_weapon(p, g);
	p->ttl = PROJECTILE_TTL;
	p->active = true;
	g->hud.ammo -= ammo_cost;
	g->hud.weapon_flash = WEAPON_FLASH_TIME;
	play_sound_effect(g, "shoot");
}

static void	update_one_projectile(t_projectile *p, t_game *g, double step)
{
	t_position	next;

	next.x = p->pos.x + p->velocity.x * step;
	next.y = p->pos.y + p->velocity.y * step;
	if (hit_sprite(g, p, next) || hit_wall(g, next))
	{
		p->active = false;
		return ;
	}
	p->pos = next;
	p->ttl -= step;
	if (p->ttl <= 0.0)
		p->active = false;
}

bool	update_projectiles(t_game *g)
{
	bool	active;
	double	step;
	int		i;

	active = false;
	step = g->delta_time;
	if (step <= 0.0 || step > 0.05)
		step = 0.016;
	i = 0;
	while (i < PROJECTILE_MAX)
	{
		if (g->projectiles[i].active)
		{
			update_one_projectile(&g->projectiles[i], g, step);
			if (g->projectiles[i].active)
				active = true;
		}
		i++;
	}
	return (active);
}
