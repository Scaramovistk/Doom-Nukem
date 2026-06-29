/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player_pos.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:25 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:27 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	update_player_pos(t_player *p, t_game *g)
{
	double		vertical_move_step;
	double		lateral_move_step;
	t_position	new_pos;
	double		add;

	vertical_move_step = p->vertical_move * MOVEMENT_SPEED;
	new_pos.x = p->pos.x + vertical_move_step * cos(p->orientation);
	new_pos.y = p->pos.y + vertical_move_step * sin(p->orientation);
	lateral_move_step = p->lateral_move * MOVEMENT_SPEED;
	add = lateral_move_step * cos(p->orientation + M_PI_2);
	new_pos.x += add * LATERAL_MOVE_RATIO;
	add = lateral_move_step * sin(p->orientation + M_PI_2);
	new_pos.y += add * LATERAL_MOVE_RATIO;
	if (is_position_legal(new_pos, g))
		p->pos = new_pos;
	p->orientation += normalize_angle(p->rotation_move * ROTATION_SPEED);
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
		if (block == WALL)
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
