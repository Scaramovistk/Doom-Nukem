/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_fire.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

static void	setup_projectile_motion(t_projectile *p, t_game *g,
		t_shot_spec spec)
{
	p->pos = spec.origin;
	p->z = get_floor_z_at(g, spec.origin) + PLAYER_STAND_HEIGHT;
	if (!spec.from_enemy)
		p->z = g->player.z + g->player.eye_height;
	p->pos.x += cos(spec.angle) * 0.35;
	p->pos.y += sin(spec.angle) * 0.35;
	p->velocity.x = cos(spec.angle) * PROJECTILE_SPEED;
	p->velocity.y = sin(spec.angle) * PROJECTILE_SPEED;
}

int	fire_projectile_from(t_game *g, t_shot_spec spec)
{
	t_projectile	*p;
	int				i;

	i = 0;
	while (i < PROJECTILE_MAX && g->projectiles[i].active)
		i++;
	if (i == PROJECTILE_MAX)
		return (-1);
	p = &g->projectiles[i];
	setup_projectile_motion(p, g, spec);
	p->damage = spec.damage;
	p->size = PROJECTILE_SIZE;
	if (spec.from_enemy)
		p->color = RED;
	else
		p->color = YELLOW;
	p->ttl = PROJECTILE_TTL;
	p->active = true;
	p->from_enemy = spec.from_enemy;
	return (i);
}

void	fire_projectile(t_game *g)
{
	int			ammo_cost;
	int			index;
	t_shot_spec	spec;

	ammo_cost = weapon_ammo_cost(g);
	if (g->hud.magazine[g->hud.selected_weapon] < ammo_cost)
	{
		if (g->hud.inventory[ITEM_AMMO] > 0)
			show_message(g, "RELOAD FROM AMMO INVENTORY", MESSAGE_DISPLAY_TIME);
		else
			show_message(g, "NO AMMO", MESSAGE_DISPLAY_TIME);
		return ;
	}
	spec = (t_shot_spec){g->player.pos, g->player.orientation, 10, false};
	index = fire_projectile_from(g, spec);
	if (index < 0)
		return ;
	setup_projectile_weapon(&g->projectiles[index], g);
	g->hud.magazine[g->hud.selected_weapon] -= ammo_cost;
	g->hud.weapon_flash = WEAPON_FLASH_TIME;
	play_sound_effect(g, "shoot");
}
