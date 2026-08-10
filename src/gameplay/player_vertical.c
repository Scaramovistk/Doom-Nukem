/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_vertical.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	player_has_free_movement(t_player *p)
{
	return (p->is_flying || p->is_swimming);
}

void	clamp_player_z(t_player *p, t_game *g)
{
	double	floor_z;
	double	max_z;

	floor_z = get_floor_z_at(g, p->pos);
	max_z = get_ceiling_z_at(g, p->pos) - p->eye_height;
	if (max_z > floor_z + PLAYER_MAX_Z)
		max_z = floor_z + PLAYER_MAX_Z;
	if (p->z < floor_z)
		p->z = floor_z;
	else if (p->z > max_z)
		p->z = max_z;
}

void	toggle_fly_mode(t_player *p, t_game *g)
{
	double	floor_z;

	floor_z = get_floor_z_at(g, p->pos);
	p->is_flying = !p->is_flying;
	p->z_velocity = 0;
	p->on_ground = !p->is_flying;
	if (!p->is_flying)
	{
		p->fly_move = 0;
		if (p->z <= floor_z)
		{
			p->z = floor_z;
			p->on_ground = true;
		}
		else
			p->on_ground = false;
	}
	else
		show_message(g, "JETPACK ENGAGED", 1.0);
	if (!p->is_flying)
		show_message(g, "JETPACK DISENGAGED", 1.0);
}

void	jump_player(t_player *p)
{
	if (!p->on_ground || p->is_crouching || player_has_free_movement(p))
		return ;
	p->z_velocity = PLAYER_JUMP_SPEED;
	p->on_ground = false;
}
