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
	if (g->map.has_flag)
		show_message(g, "CAPTURE THE FLAG - RETURN IT TO BASE", MESSAGE_DISPLAY_TIME);
	else if (g->map.exit_count)
		show_message(g, "MISSION START REACH EXIT", MESSAGE_DISPLAY_TIME);
	else
		show_message(g, "MISSION START", MESSAGE_DISPLAY_TIME);
}

static void	complete_level(t_game *g)
{
	g->level.completed = true;
	g->level.end_timer = LEVEL_END_DELAY;
	g->hud.score += 100;
	if (g->campaign_mode && g->campaign_level)
	{
		g->story_visible = true;
		g->story_is_debrief = true;
	}
	if (g->map.has_flag)
		show_message(g, "CONGRATS! FLAG RETURNED", LEVEL_END_DELAY);
	else
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
	if (g->story_visible && g->story_is_debrief)
		return (true);
	g->level.end_timer -= g->delta_time;
	if (g->level.end_timer <= 0.0)
	{
		if (g->level.completed && g->level.next_level[0])
			load_next_level(g);
		else
			stop_game(g);
	}
	return (true);
}

void	load_next_level(t_game *g)
{
	char	path[LINE_SIZE];

	ft_strlcpy(path, g->level.next_level, LINE_SIZE);
	if (!load_level_path(g, path))
		stop_game(g);
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
	if (g->map.has_flag)
	{
		if (!g->map.flag_carried && pow(g->player.pos.x - g->map.flag_pos.x, 2)
			+ pow(g->player.pos.y - g->map.flag_pos.y, 2)
			<= ITEM_PICKUP_RADIUS * ITEM_PICKUP_RADIUS)
		{
			g->map.flag_carried = true;
			show_message(g, "FLAG TAKEN - RETURN TO BASE", MESSAGE_DISPLAY_TIME);
			play_sound_effect(g, "pickup");
		}
		if (g->map.flag_carried && pow(g->player.pos.x - g->map.flag_base.x, 2)
			+ pow(g->player.pos.y - g->map.flag_base.y, 2)
			<= ITEM_PICKUP_RADIUS * ITEM_PICKUP_RADIUS)
			complete_level(g);
		return (g->level.completed);
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
