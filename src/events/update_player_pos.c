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

static bool	has_free_movement(t_player *p)
{
	return (p->is_flying || p->is_swimming);
}

static void	clamp_player_z(t_player *p, t_game *g)
{
	double	floor_z;
	double	max_z;

	floor_z = get_floor_z_at(g, p->pos);
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
}

void	jump_player(t_player *p)
{
	if (!p->on_ground || p->is_crouching || has_free_movement(p))
		return ;
	p->z_velocity = PLAYER_JUMP_SPEED;
	p->on_ground = false;
}

static void	update_vertical_physics(t_player *p, t_game *g)
{
	double	floor_z;
	double	ceiling_z;

	floor_z = get_floor_z_at(g, p->pos);
	ceiling_z = get_ceiling_z_at(g, p->pos);
	if (has_free_movement(p))
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
	if (!p->on_ground)
	{
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
	if (p->is_crouching)
		p->eye_height = PLAYER_CROUCH_HEIGHT;
	else
		p->eye_height = PLAYER_STAND_HEIGHT;
}

static double	get_pitch_climb(t_player *p, double move_step)
{
	return (move_step * (p->pitch / PITCH_LIMIT) * FLY_PITCH_CLIMB_RATIO);
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
	if (has_free_movement(p))
	{
		p->z += get_pitch_climb(p, vertical_move_step);
		clamp_player_z(p, g);
	}
	p->orientation = normalize_angle(p->orientation + (p->key_rotation_move
				+ p->rotation_move) * ROTATION_SPEED);
	update_pitch(p);
	update_vertical_physics(p, g);
	clear_mouse_move(p);
}

bool	is_position_legal(t_position pos, t_game *g)
{
	const t_position	check_pos[4] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
	t_coord				cell;
	t_block				block;
	int					i;
	double				current_floor;
	double				target_floor;

	current_floor = get_floor_z_at(g, g->player.pos);
	target_floor = get_floor_z_at(g, pos);
	if (segment_blocks_position(g, pos))
		return (false);
	if (target_floor > current_floor + PLAYER_STEP_HEIGHT)
		return (false);
	if (get_ceiling_z_at(g, pos) - target_floor < g->player.eye_height)
		return (false);
	i = 0;
	while (i < 4)
	{
		cell = get_adjacent_cell(pos, check_pos[i]);
		block = g->map.grid[cell.y][cell.x];
		if (block == WALL || block == TRANSPARENT_WALL || block == DECAL_WALL)
			return (false);
		if (block == DOOR && !is_door_open(cell, g->map.doors))
			return (false);
		if (is_item_blocking(cell, g))
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
