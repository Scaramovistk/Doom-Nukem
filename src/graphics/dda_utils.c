/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:32 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:35 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	init_dda(t_dda *dda, t_ray *ray, t_game *g)
{
	dda->map.x = (int)g->player.pos.x;
	dda->map.y = (int)g->player.pos.y;
	dda->delta_dist.x = fabs(1 / ray->dir.x);
	dda->delta_dist.y = fabs(1 / ray->dir.y);
	calculate_step_and_side_dist(dda, ray, g);
	dda->ray = ray;
}

void	calculate_step_and_side_dist(t_dda *dda, t_ray *ray, t_game *g)
{
	if (ray->dir.x < 0)
	{
		dda->step.x = -1;
		dda->side_dist.x = (g->player.pos.x - dda->map.x) * dda->delta_dist.x;
	}
	else
	{
		dda->step.x = 1;
		dda->side_dist.x = (dda->map.x + 1.0 - g->player.pos.x)
			* dda->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		dda->step.y = -1;
		dda->side_dist.y = (g->player.pos.y - dda->map.y) * dda->delta_dist.y;
	}
	else
	{
		dda->step.y = 1;
		dda->side_dist.y = (dda->map.y + 1.0 - g->player.pos.y)
			* dda->delta_dist.y;
	}
}

void	apply_cross_distance(t_dda *dda, double *distance, int side, t_game *g)
{
	t_ray	*ray;

	ray = dda->ray;
	if (side == 0)
		*distance = (dda->map.x - g->player.pos.x + (1 - dda->step.x) / 2)
			/ ray->dir.x;
	else
		*distance = (dda->map.y - g->player.pos.y + (1 - dda->step.y) / 2)
			/ ray->dir.y;
	*distance *= cos(g->player.orientation - ray->angle);
}
