/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:43 by gscarama          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_scene(t_game *g)
{
	t_ray	rays[WIN_WIDTH];
	double	z_buffer[WIN_WIDTH];

	draw_floor_ceiling(g);
	draw_all_rays_threaded(rays, g, z_buffer);
	draw_sprites(g, z_buffer, rays);
	draw_projectiles(g, z_buffer, rays);
}
