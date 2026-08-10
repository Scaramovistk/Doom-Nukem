/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_projectiles(t_game *g, double *z_buffer, t_ray *rays)
{
	t_sprite_draw	s;
	int				i;

	i = 0;
	while (i < PROJECTILE_MAX)
	{
		if (g->projectiles[i].active && bsp_position_visible(g,
				g->projectiles[i].pos))
		{
			init_projectile_draw(&s, &g->projectiles[i], g);
			if (s.transform_y > 0 && s.screen_x >= 0 && s.screen_x < WIN_WIDTH
				&& s.transform_y < z_buffer[s.screen_x])
				draw_projectile_dot(g, &s, &g->projectiles[i], rays);
		}
		i++;
	}
}
