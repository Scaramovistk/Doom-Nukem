/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   height_step.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	get_sector_id_at_cell(t_game *g, t_coord cell)
{
	int	id;

	id = 0;
	if (g->map.sector_grid && cell.x >= 0 && cell.y >= 0
		&& cell.x < g->map.width && cell.y < g->map.height)
		id = g->map.sector_grid[cell.y][cell.x];
	if (id < 0 || id >= SECTOR_MAX || !g->map.sectors[id].active)
		id = 0;
	return (id);
}

static bool	sector_height_differs(t_game *g, t_position near, t_position far)
{
	return (fabs(get_floor_z_at(g, near) - get_floor_z_at(g, far)) > 0.01
		|| fabs(get_ceiling_z_at(g, near) - get_ceiling_z_at(g, far)) > 0.01);
}

void	record_height_step(t_dda *dda, t_ray *ray, t_game *g, int *prev_sector)
{
	int			new_sector;
	int			n;
	double		distance;
	t_position	near;
	t_position	far;

	new_sector = get_sector_id_at_cell(g, dda->map);
	apply_cross_distance(dda, &distance, dda->side, g);
	near = ray_world_pos(ray, distance, g);
	far.x = near.x + ray->dir.x * 0.02;
	far.y = near.y + ray->dir.y * 0.02;
	if (new_sector != *prev_sector && ray->height_step_count < HEIGHT_STEP_MAX
		&& sector_height_differs(g, near, far))
	{
		n = ray->height_step_count++;
		ray->height_steps[n].distance = distance;
		ray->height_steps[n].side = dda->side;
		ray->height_steps[n].near_sector = *prev_sector;
		ray->height_steps[n].far_sector = new_sector;
	}
	*prev_sector = new_sector;
}

int	band_sector(t_ray *ray, int i)
{
	if (i == 0)
		return (ray->height_steps[0].near_sector);
	return (ray->height_steps[i - 1].far_sector);
}

double	step_far_d(t_ray *ray, int i)
{
	if (i < ray->height_step_count)
		return (ray->height_steps[i].distance);
	return (ray->distance);
}
