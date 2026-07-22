/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level_flow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_on_zone(t_coord cell, t_coord *zones, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (zones[i].x == cell.x && zones[i].y == cell.y)
			return (true);
		i++;
	}
	return (false);
}

static int	active_items(t_game *g)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active)
			count++;
		i++;
	}
	return (count);
}

void	start_level_flow(t_game *g)
{
	if (g->level.started)
		return ;
	g->level.started = true;
	g->level.required_items = active_items(g);
	if (g->map.exit_count)
		show_message(g, "MISSION START REACH EXIT", MESSAGE_DISPLAY_TIME);
	else
		show_message(g, "MISSION START", MESSAGE_DISPLAY_TIME);
}

static void	complete_level(t_game *g)
{
	g->level.completed = true;
	g->level.end_timer = LEVEL_END_DELAY;
	g->hud.score += 100;
	show_message(g, "MISSION COMPLETE", LEVEL_END_DELAY);
	play_sound_effect(g, "mission_complete");
}

static void	fail_level(t_game *g)
{
	g->level.failed = true;
	g->level.end_timer = LEVEL_END_DELAY;
	show_message(g, "MISSION FAILED", LEVEL_END_DELAY);
	play_sound_effect(g, "mission_failed");
}

static bool	update_end_state(t_game *g)
{
	if (!g->level.completed && !g->level.failed)
		return (false);
	g->level.end_timer -= g->delta_time;
	if (g->level.end_timer <= 0.0)
		stop_game(g);
	return (true);
}

bool	update_level_flow(t_game *g)
{
	t_coord	player_cell;

	if (!g->level.started)
		start_level_flow(g);
	if (update_end_state(g))
		return (true);
	if (g->hud.health <= 0)
	{
		fail_level(g);
		return (true);
	}
	if (!g->map.exit_count)
		return (false);
	player_cell.x = (int)g->player.pos.x;
	player_cell.y = (int)g->player.pos.y;
	if (!active_items(g) && is_on_zone(player_cell,
			g->map.exit_zones, g->map.exit_count))
	{
		complete_level(g);
		return (true);
	}
	return (false);
}
