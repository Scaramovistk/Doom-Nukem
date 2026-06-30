/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:37 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:38 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	progress_dda(t_dda *dda)
{
	if (dda->side_dist.x < dda->side_dist.y)
	{
		dda->side_dist.x += dda->delta_dist.x;
		dda->map.x += dda->step.x;
		dda->side = 0;
	}
	else
	{
		dda->side_dist.y += dda->delta_dist.y;
		dda->map.y += dda->step.y;
		dda->side = 1;
	}
}

bool	wall_collision(t_dda *dda, t_ray *ray, t_game *g)
{
	if (g->map.grid[dda->map.y][dda->map.x] == WALL)
	{
		ray->side = dda->side;
		apply_cross_distance(dda, &ray->distance, ray->side, g);
		return (true);
	}
	return (false);
}

bool	door_collision(t_dda *dda, t_ray *ray, t_game *g)
{
	t_door	*door;
	double	door_distance;

	if (g->map.grid[dda->map.y][dda->map.x] == DOOR)
	{
		door = &g->map.doors[dda->map.y][dda->map.x];
		if (door->opening_state < 1.0)
		{
			apply_cross_distance(dda, &door_distance, dda->side, g);
			if (ray->hit_door == NULL || door_distance < ray->door_distance)
			{
				ray->hit_door = door;
				ray->door_side = dda->side;
				ray->door_distance = door_distance;
			}
			return (true);
		}
	}
	return (false);
}

void	perform_dda(t_dda *dda, t_ray *ray, t_game *g)
{
	bool	hit;

	hit = false;
	ray->hit_door = NULL;
	while (!hit)
	{
		progress_dda(dda);
		door_collision(dda, ray, g);
		if (wall_collision(dda, ray, g))
		{
			hit = true;
		}
	}
}
