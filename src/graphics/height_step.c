/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   height_step.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by codex           ###   ########.fr       */
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

static bool	sector_height_differs(t_game *g, int a, int b)
{
	t_sector	*sa;
	t_sector	*sb;

	sa = &g->map.sectors[a];
	sb = &g->map.sectors[b];
	return (fabs(sa->floor_z - sb->floor_z) > 0.01
		|| fabs(sa->ceil_z - sb->ceil_z) > 0.01);
}

void	record_height_step(t_dda *dda, t_ray *ray, t_game *g, int *prev_sector)
{
	int	new_sector;
	int	n;

	new_sector = get_sector_id_at_cell(g, dda->map);
	if (new_sector != *prev_sector && ray->height_step_count < HEIGHT_STEP_MAX
		&& sector_height_differs(g, *prev_sector, new_sector))
	{
		n = ray->height_step_count++;
		apply_cross_distance(dda, &ray->height_steps[n].distance,
			dda->side, g);
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
