/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:25 by gscarama          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	update_pitch(t_player *p)
{
	p->pitch += (p->key_pitch_move + p->pitch_move) * PITCH_SPEED;
	if (p->pitch < -PITCH_LIMIT)
		p->pitch = -PITCH_LIMIT;
	else if (p->pitch > PITCH_LIMIT)
		p->pitch = PITCH_LIMIT;
}

static void	clear_mouse_move(t_player *p)
{
	if (!p->mouse_move_pending)
		return ;
	p->rotation_move = 0;
	p->pitch_move = 0;
	p->mouse_move_pending = false;
}

static double	get_pitch_climb(t_player *p, double move_step)
{
	return (move_step * (p->pitch / PITCH_LIMIT) * FLY_PITCH_CLIMB_RATIO);
}

static t_position	get_new_position(t_player *p)
{
	t_position	pos;
	double		move_speed;
	double		forward;
	double		lateral;

	move_speed = MOVEMENT_SPEED;
	if (p->is_running && !p->is_crouching)
		move_speed *= RUN_SPEED_MULTIPLIER;
	forward = p->vertical_move * move_speed;
	lateral = p->lateral_move * move_speed * LATERAL_MOVE_RATIO;
	pos.x = p->pos.x + forward * cos(p->orientation);
	pos.y = p->pos.y + forward * sin(p->orientation);
	pos.x += lateral * cos(p->orientation + M_PI_2);
	pos.y += lateral * sin(p->orientation + M_PI_2);
	return (pos);
}

void	update_player_pos(t_player *p, t_game *g)
{
	t_position	new_pos;
	double		forward;

	new_pos = get_new_position(p);
	forward = p->vertical_move * MOVEMENT_SPEED;
	if (p->is_running && !p->is_crouching)
		forward *= RUN_SPEED_MULTIPLIER;
	if (is_position_legal(new_pos, g))
		p->pos = new_pos;
	if (player_has_free_movement(p))
	{
		p->z += get_pitch_climb(p, forward);
		clamp_player_z(p, g);
	}
	p->orientation = normalize_angle(p->orientation + (p->key_rotation_move
				+ p->rotation_move) * ROTATION_SPEED);
	update_pitch(p);
	update_vertical_physics(p, g);
	clear_mouse_move(p);
}
