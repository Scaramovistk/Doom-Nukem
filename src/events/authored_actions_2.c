/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authored_actions_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	authored_action_line_valid(const char *line)
{
	t_authored_action	action;

	return (parse_authored_action(line, &action));
}

bool	trigger_exists(t_game *g, t_coord trigger)
{
	int	i;

	i = 0;
	while (i < g->map.switch_count)
	{
		if (g->map.switches[i].x == trigger.x
			&& g->map.switches[i].y == trigger.y)
			return (true);
		i++;
	}
	return (false);
}

bool	texture_available(t_game *g, int texture)
{
	if (texture < 0 || texture >= TEXTURES_NB)
		return (false);
	if (texture == SPRITE_T && g->assets.has_sprite_frames)
		return (true);
	return (g->assets.textures[texture].source != NULL);
}

bool	action_target_valid(t_game *g, t_authored_action *a)
{
	if (!trigger_exists(g, a->trigger))
		return (false);
	if (a->type == AUTH_ACTION_BLOCK)
		return (block_target_valid(g, a));
	if (a->type >= AUTH_ACTION_FLOOR && a->type <= AUTH_ACTION_LIGHT)
		return (a->target >= 0 && a->target < SECTOR_MAX
			&& g->map.sectors[a->target].active);
	if (a->type == AUTH_ACTION_TEXTURE_SWAP)
		return (texture_available(g, a->target) && texture_available(g,
				a->value));
	if (a->type >= AUTH_ACTION_OBJECT_MOVE
		&& a->type <= AUTH_ACTION_OBJECT_TEXTURE)
		return (object_target_valid(g, a));
	if (a->type == AUTH_ACTION_WALL_MOVE || a->type == AUTH_ACTION_WALL_TEXTURE)
		return (wall_target_valid(g, a));
	return (true);
}

bool	add_authored_action(t_game *g, const char *line)
{
	t_authored_action	action;

	if (g->map.action_count >= AUTHORED_ACTION_MAX
		|| !parse_authored_action(line, &action) || !action_target_valid(g,
			&action))
		return (false);
	g->map.actions[g->map.action_count++] = action;
	return (true);
}
