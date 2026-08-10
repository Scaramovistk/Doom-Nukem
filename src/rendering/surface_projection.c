/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_projection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	init_floor_cast(t_floor_cast *cast, t_game *g)
{
	double	dir_x;
	double	dir_y;
	double	plane_len;

	dir_x = cos(g->player.orientation);
	dir_y = sin(g->player.orientation);
	plane_len = tan(deg_to_rad(FOV) / 2.0);
	cast->ray_dir_x0 = dir_x + dir_y * plane_len;
	cast->ray_dir_y0 = dir_y - dir_x * plane_len;
	cast->ray_dir_x1 = dir_x - dir_y * plane_len;
	cast->ray_dir_y1 = dir_y + dir_x * plane_len;
}

void	set_surface_row(int y, int horizon, t_floor_cast *cast, t_game *g)
{
	int		row_offset;
	double	camera_height;
	double	eye_z;

	row_offset = y - horizon;
	if (row_offset < 0)
		row_offset = -row_offset;
	eye_z = g->player.z + g->player.eye_height;
	if (y > horizon)
		camera_height = eye_z - get_floor_z_at(g, g->player.pos);
	else
		camera_height = get_ceiling_z_at(g, g->player.pos) - eye_z;
	if (camera_height < 0.05)
		camera_height = 0.05;
	cast->row_distance = (camera_height * WIN_HEIGHT) / row_offset;
	cast->step_x = cast->row_distance * (cast->ray_dir_x1 - cast->ray_dir_x0)
		/ WIN_WIDTH;
	cast->step_y = cast->row_distance * (cast->ray_dir_y1 - cast->ray_dir_y0)
		/ WIN_WIDTH;
	cast->floor_x = g->player.pos.x + cast->row_distance * cast->ray_dir_x0;
	cast->floor_y = g->player.pos.y + cast->row_distance * cast->ray_dir_y0;
}
