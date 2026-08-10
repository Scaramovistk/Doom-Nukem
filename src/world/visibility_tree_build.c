/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visibility_tree_build.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	build_node(t_game *g, t_bsp_node bounds);

static void	split_x(t_game *g, t_bsp_node *node)
{
	int	middle;

	middle = (node->min_x + node->max_x) / 2;
	node->left = build_node(g, (t_bsp_node){node->min_x, node->min_y,
			middle, node->max_y, -1, -1});
	node->right = build_node(g, (t_bsp_node){middle + 1, node->min_y,
			node->max_x, node->max_y, -1, -1});
}

static void	split_y(t_game *g, t_bsp_node *node)
{
	int	middle;

	middle = (node->min_y + node->max_y) / 2;
	node->left = build_node(g, (t_bsp_node){node->min_x, node->min_y,
			node->max_x, middle, -1, -1});
	node->right = build_node(g, (t_bsp_node){node->min_x, middle + 1,
			node->max_x, node->max_y, -1, -1});
}

static int	build_node(t_game *g, t_bsp_node bounds)
{
	t_bsp_node	*node;
	int			index;

	index = g->map.bsp_node_count++;
	node = &g->map.bsp_nodes[index];
	*node = bounds;
	if (node->min_x == node->max_x && node->min_y == node->max_y)
		return (index);
	if (node->max_x - node->min_x >= node->max_y - node->min_y)
		split_x(g, node);
	else
		split_y(g, node);
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
	build_node(g, (t_bsp_node){0, 0, g->map.width - 1,
		g->map.height - 1, -1, -1});
	update_bsp_visibility(g);
}
