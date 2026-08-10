/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_visibility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	node_in_view(t_game *g, t_bsp_node *node)
{
	t_position	center;
	t_position	delta;
	double		distance;
	double		radius;
	double		angle;

	if (g->player.pos.x >= node->min_x && g->player.pos.x <= node->max_x + 1
		&& g->player.pos.y >= node->min_y && g->player.pos.y <= node->max_y + 1)
		return (true);
	center.x = (node->min_x + node->max_x + 1) / 2.0;
	center.y = (node->min_y + node->max_y + 1) / 2.0;
	delta.x = center.x - g->player.pos.x;
	delta.y = center.y - g->player.pos.y;
	distance = sqrt(delta.x * delta.x + delta.y * delta.y);
	radius = sqrt(pow((node->max_x - node->min_x + 1) / 2.0, 2)
			+ pow((node->max_y - node->min_y + 1) / 2.0, 2));
	if (distance <= radius)
		return (true);
	angle = atan2(delta.y, delta.x) - g->player.orientation;
	angle = fabs(atan2(sin(angle), cos(angle)));
	return (angle <= deg_to_rad(FOV / 2.0) + asin(radius / distance));
}

static void	mark_visible_nodes(t_game *g, int index)
{
	t_bsp_node	*node;

	node = &g->map.bsp_nodes[index];
	if (!node_in_view(g, node))
		return ;
	if (node->left < 0)
	{
		g->map.visible_tiles[node->min_y * g->map.width + node->min_x] = true;
		return ;
	}
	mark_visible_nodes(g, node->left);
	mark_visible_nodes(g, node->right);
}

void	update_bsp_visibility(t_game *g)
{
	if (!g->map.bsp_nodes || !g->map.visible_tiles)
		return ;
	if (g->map.visibility_valid && g->map.visibility_origin.x == g->player.pos.x
		&& g->map.visibility_origin.y == g->player.pos.y
		&& g->map.visibility_angle == g->player.orientation)
		return ;
	ft_bzero(g->map.visible_tiles, g->map.width * g->map.height * sizeof(bool));
	mark_visible_nodes(g, 0);
	g->map.visibility_origin = g->player.pos;
	g->map.visibility_angle = g->player.orientation;
	g->map.visibility_valid = true;
}

bool	bsp_tile_visible(t_game *g, t_coord cell)
{
	if (!g->map.visible_tiles || cell.x < 0 || cell.y < 0
		|| cell.x >= g->map.width || cell.y >= g->map.height)
		return (true);
	return (g->map.visible_tiles[cell.y * g->map.width + cell.x]);
}

bool	bsp_position_visible(t_game *g, t_position pos)
{
	return (bsp_tile_visible(g, (t_coord){(int)pos.x, (int)pos.y}));
}
