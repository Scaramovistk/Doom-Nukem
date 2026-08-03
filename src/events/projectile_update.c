/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_update.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	hit_player(t_game *g, t_projectile *p, t_position pos)
{
	double	dx;
	double	dy;

	if (!p->from_enemy)
		return (false);
	dx = g->player.pos.x - pos.x;
	dy = g->player.pos.y - pos.y;
	if (dx * dx + dy * dy > PROJECTILE_HIT_RADIUS * PROJECTILE_HIT_RADIUS)
		return (false);
	g->hud.health -= p->damage;
	if (g->hud.health < 0)
		g->hud.health = 0;
	return (true);
}

static void	update_one_projectile(t_projectile *p, t_game *g, double step)
{
	t_position	next;

	next.x = p->pos.x + p->velocity.x * step;
	next.y = p->pos.y + p->velocity.y * step;
	if (hit_wall(g, p, next) || hit_player(g, p, next)
		|| hit_sprite(g, p, next))
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
