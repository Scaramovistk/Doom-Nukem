/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static double	dist_sq(t_position a, t_position b)
{
	double	dx;
	double	dy;

	dx = a.x - b.x;
	dy = a.y - b.y;
	return (dx * dx + dy * dy);
}

static bool	enemy_cell_legal(t_game *g, t_position pos)
{
	t_coord	cell;
	t_block	block;

	cell.x = (int)pos.x;
	cell.y = (int)pos.y;
	if (!is_in_bounds(cell, g))
		return (false);
	block = g->map.grid[cell.y][cell.x];
	if (block == WALL || block == TRANSPARENT_WALL || block == DECAL_WALL)
		return (false);
	if (block == DOOR && !is_door_open(cell, g->map.doors))
		return (false);
	return (true);
}

static void	remove_enemy_sprite(t_game *g, t_enemy *enemy)
{
	int	last;
	int	i;

	last = g->map.sprite_count - 1;
	if (enemy->sprite_index < 0 || enemy->sprite_index > last)
		return ;
	if (enemy->sprite_index != last)
	{
		g->map.sprites[enemy->sprite_index] = g->map.sprites[last];
		i = 0;
		while (i < g->map.item_count)
		{
			if (g->map.items[i].sprite_index == last)
				g->map.items[i].sprite_index = enemy->sprite_index;
			i++;
		}
		i = 0;
		while (i < g->map.enemy_count)
		{
			if (g->map.enemies[i].active
				&& g->map.enemies[i].sprite_index == last)
				g->map.enemies[i].sprite_index = enemy->sprite_index;
			i++;
		}
	}
	g->map.sprite_count--;
	enemy->sprite_index = -1;
}

bool	damage_enemy_at_sprite(t_game *g, int sprite_index, int damage)
{
	t_enemy	*enemy;
	int		i;

	i = 0;
	while (i < g->map.enemy_count)
	{
		enemy = &g->map.enemies[i];
		if (enemy->active && enemy->sprite_index == sprite_index)
		{
			enemy->health -= damage;
			if (enemy->health <= 0)
			{
				enemy->active = false;
				remove_enemy_sprite(g, enemy);
				g->hud.score += 25;
				show_message(g, "TARGET DOWN", 1.2);
			}
			return (true);
		}
		i++;
	}
	return (false);
}

static bool	enemy_attack(t_enemy *enemy, t_game *g, double distance)
{
	if (distance > ENEMY_ATTACK_RANGE * ENEMY_ATTACK_RANGE)
		return (false);
	enemy->attack_timer -= g->delta_time;
	if (enemy->attack_timer <= 0.0)
	{
		g->hud.health -= ENEMY_ATTACK_DAMAGE;
		if (g->hud.health < 0)
			g->hud.health = 0;
		enemy->attack_timer = ENEMY_ATTACK_DELAY;
		return (true);
	}
	return (false);
}

static bool	enemy_chase(t_enemy *enemy, t_game *g, double distance)
{
	t_position	next;
	double		len;
	double		step;

	if (distance > ENEMY_ALERT_RANGE * ENEMY_ALERT_RANGE)
		return (false);
	len = sqrt(distance);
	if (len <= 0.001)
		return (false);
	step = ENEMY_MOVE_SPEED * g->delta_time;
	next.x = enemy->pos.x + ((g->player.pos.x - enemy->pos.x) / len) * step;
	next.y = enemy->pos.y + ((g->player.pos.y - enemy->pos.y) / len) * step;
	if (!enemy_cell_legal(g, next))
		return (false);
	enemy->pos = next;
	if (enemy->sprite_index >= 0 && enemy->sprite_index < g->map.sprite_count)
		g->map.sprites[enemy->sprite_index] = enemy->pos;
	return (true);
}

bool	update_enemies(t_game *g)
{
	bool	updated;
	double	distance;
	int		i;

	updated = false;
	i = 0;
	while (i < g->map.enemy_count)
	{
		if (g->map.enemies[i].active)
		{
			distance = dist_sq(g->map.enemies[i].pos, g->player.pos);
			if (enemy_attack(&g->map.enemies[i], g, distance))
				updated = true;
			else if (enemy_chase(&g->map.enemies[i], g, distance))
				updated = true;
		}
		i++;
	}
	return (updated);
}
