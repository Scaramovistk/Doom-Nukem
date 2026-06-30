/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:13:10 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:13:16 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	cast_all_rays(t_ray *rays, t_game *g)
{
	double	start_angle;
	double	angle_step;
	int		i;
	double	ray_angle;

	start_angle = g->player.orientation - deg_to_rad(FOV / 2);
	angle_step = deg_to_rad(FOV) / WIN_WIDTH;
	i = 0;
	while (i < WIN_WIDTH)
	{
		ray_angle = start_angle + i * angle_step;
		cast_one_ray(&rays[i], ray_angle, g);
		i++;
	}
}

void	cast_one_ray(t_ray *ray, double angle, t_game *g)
{
	t_dda	dda;

	angle = normalize_angle(angle);
	ray->angle = angle;
	ray->dir.x = cos(angle);
	ray->dir.y = sin(angle);
	ray->hit_door = false;
	init_dda(&dda, ray, g);
	perform_dda(&dda, ray, g);
}
