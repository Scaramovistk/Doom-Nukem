/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_gravity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	update_airborne(t_player *p, double floor_z, double ceiling_z)
{
	if (p->on_ground)
		return ;
	p->z += p->z_velocity;
	p->z_velocity -= PLAYER_GRAVITY;
	if (p->z + p->eye_height > ceiling_z)
	{
		p->z = ceiling_z - p->eye_height;
		p->z_velocity = 0;
	}
	if (p->z <= floor_z)
	{
		p->z = floor_z;
		p->z_velocity = 0;
		p->on_ground = true;
	}
}

void	update_vertical_physics(t_player *p, t_game *g)
{
	double	floor_z;
	double	ceiling_z;

	floor_z = get_floor_z_at(g, p->pos);
	ceiling_z = get_ceiling_z_at(g, p->pos);
	if (player_has_free_movement(p))
	{
		p->z += p->fly_move * FLY_VERTICAL_SPEED;
		clamp_player_z(p, g);
		p->z_velocity = 0;
		p->on_ground = false;
		return ;
	}
	if (p->on_ground && p->z > floor_z + PLAYER_STEP_HEIGHT)
		p->on_ground = false;
	else if (p->on_ground)
		p->z = floor_z;
	update_airborne(p, floor_z, ceiling_z);
	if (p->is_crouching)
		p->eye_height = PLAYER_CROUCH_HEIGHT;
	else
		p->eye_height = PLAYER_STAND_HEIGHT;
}
