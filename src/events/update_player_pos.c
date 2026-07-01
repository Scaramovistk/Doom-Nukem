/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player_pos.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:25 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:27 by gscarama         ###   ########.fr       */
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

void	jump_player(t_player *p)
{
	if (!p->on_ground || p->is_crouching)
		return ;
	p->z_velocity = PLAYER_JUMP_SPEED;
	p->on_ground = false;
}

static void	update_vertical_physics(t_player *p)
{
	if (!p->on_ground)
	{
		p->z += p->z_velocity;
		p->z_velocity -= PLAYER_GRAVITY;
		if (p->z <= PLAYER_FLOOR_Z)
		{
			p->z = PLAYER_FLOOR_Z;
			p->z_velocity = 0;
			p->on_ground = true;
		}
	}
	if (p->is_crouching)
		p->eye_height = PLAYER_CROUCH_HEIGHT;
	else
		p->eye_height = PLAYER_STAND_HEIGHT;
}

void	update_player_pos(t_player *p, t_game *g)
{
	double		vertical_move_step;
	double		lateral_move_step;
	t_position	new_pos;
	double		add;
	double		move_speed;

	move_speed = MOVEMENT_SPEED;
	if (p->is_running && !p->is_crouching)
		move_speed *= RUN_SPEED_MULTIPLIER;
	vertical_move_step = p->vertical_move * move_speed;
	new_pos.x = p->pos.x + vertical_move_step * cos(p->orientation);
	new_pos.y = p->pos.y + vertical_move_step * sin(p->orientation);
	lateral_move_step = p->lateral_move * move_speed;
	add = lateral_move_step * cos(p->orientation + M_PI_2);
	new_pos.x += add * LATERAL_MOVE_RATIO;
	add = lateral_move_step * sin(p->orientation + M_PI_2);
	new_pos.y += add * LATERAL_MOVE_RATIO;
	if (is_position_legal(new_pos, g))
		p->pos = new_pos;
	p->orientation = normalize_angle(p->orientation + (p->key_rotation_move
				+ p->rotation_move) * ROTATION_SPEED);
	update_pitch(p);
	update_vertical_physics(p);
	clear_mouse_move(p);
}

bool	is_position_legal(t_position pos, t_game *g)
{
	const t_position	check_pos[4] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
	t_coord				cell;
	t_block				block;
	int					i;

	i = 0;
	while (i < 4)
	{
		cell = get_adjacent_cell(pos, check_pos[i]);
		block = g->map.grid[cell.y][cell.x];
		if (block == WALL || block == TRANSPARENT_WALL || block == DECAL_WALL)
			return (false);
		if (block == DOOR && !is_door_open(cell, g->map.doors))
			return (false);
		i++;
	}
	return (true);
}

t_coord	get_adjacent_cell(t_position pos, const t_position add)
{
	t_coord	cell;

	cell.x = (int)(pos.x + add.x * COLLISION_SAFETY);
	cell.y = (int)(pos.y + add.y * COLLISION_SAFETY);
	return (cell);
}

bool	is_door_open(t_coord cell, t_door **doors)
{
	if (doors[cell.y][cell.x].opening_state == 1.0)
		return (true);
	return (false);
}
