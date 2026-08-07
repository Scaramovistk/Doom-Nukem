/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_visibility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           +#+#+#+#+#+   +#+           +#+#+#+#+#+   +#+    */
/*   Created: 2026/08/07 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	build_node(t_game *g, int min_x, int min_y, int max_x, int max_y)
{
	t_bsp_node	*node;
	int			index;

	index = g->map.bsp_node_count++;
	node = &g->map.bsp_nodes[index];
	node->min_x = min_x;
	node->min_y = min_y;
	node->max_x = max_x;
	node->max_y = max_y;
	node->left = -1;
	node->right = -1;
	if (min_x == max_x && min_y == max_y)
		return (index);
	if (max_x - min_x >= max_y - min_y)
	{
		node->left = build_node(g, min_x, min_y, (min_x + max_x) / 2, max_y);
		node->right = build_node(g, (min_x + max_x) / 2 + 1, min_y,
				max_x, max_y);
	}
	else
	{
		node->left = build_node(g, min_x, min_y, max_x, (min_y + max_y) / 2);
		node->right = build_node(g, min_x, (min_y + max_y) / 2 + 1,
				max_x, max_y);
	}
	return (index);
}

void	init_bsp_visibility(t_game *g)
{
	int	max_nodes;

	if (!g->map.width || !g->map.height)
		return ;
	max_nodes = g->map.width * g->map.height * 2 - 1;
	g->map.bsp_nodes = calloc_s(max_nodes, sizeof(t_bsp_node), g);
	g->map.visible_tiles = calloc_s(g->map.width * g->map.height,
			sizeof(bool), g);
	g->map.bsp_node_count = 0;
	build_node(g, 0, 0, g->map.width - 1, g->map.height - 1);
	update_bsp_visibility(g);
}

static bool	node_in_view(t_game *g, t_bsp_node *node)
{
	double	center_x;
	double	center_y;
	double	dx;
	double	dy;
	double	distance;
	double	radius;
	double	angle;

	if (g->player.pos.x >= node->min_x && g->player.pos.x <= node->max_x + 1
		&& g->player.pos.y >= node->min_y && g->player.pos.y <= node->max_y + 1)
		return (true);
	center_x = (node->min_x + node->max_x + 1) / 2.0;
	center_y = (node->min_y + node->max_y + 1) / 2.0;
	dx = center_x - g->player.pos.x;
	dy = center_y - g->player.pos.y;
	distance = sqrt(dx * dx + dy * dy);
	radius = sqrt(pow((node->max_x - node->min_x + 1) / 2.0, 2)
			+ pow((node->max_y - node->min_y + 1) / 2.0, 2));
	if (distance <= radius)
		return (true);
	angle = atan2(dy, dx) - g->player.orientation;
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
	if (g->map.visibility_valid
		&& g->map.visibility_origin.x == g->player.pos.x
		&& g->map.visibility_origin.y == g->player.pos.y
		&& g->map.visibility_angle == g->player.orientation)
		return ;
	ft_bzero(g->map.visible_tiles, g->map.width * g->map.height
		* sizeof(bool));
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
