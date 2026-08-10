/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   laptop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	player_near_laptop(t_game *g, t_laptop *laptop)
{
	double	dx;
	double	dy;

	dx = g->player.pos.x - laptop->pos.x;
	dy = g->player.pos.y - laptop->pos.y;
	return (dx * dx + dy * dy <= LAPTOP_PROXIMITY_RADIUS
		* LAPTOP_PROXIMITY_RADIUS);
}

bool	update_laptop_proximity(t_game *g)
{
	int		i;
	bool	active;

	active = false;
	i = 0;
	while (i < g->map.laptop_count)
	{
		if (player_near_laptop(g, &g->map.laptops[i]))
		{
			if (!g->map.laptops[i].player_near)
				play_sound_effect(g, LAPTOP_SOUND);
			g->map.laptops[i].player_near = true;
			active = true;
		}
		else
			g->map.laptops[i].player_near = false;
		i++;
	}
	return (active);
}
