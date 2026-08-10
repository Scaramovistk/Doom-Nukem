/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	complete_level(t_game *g)
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

void	fail_level(t_game *g)
{
	g->level.failed = true;
	g->level.end_timer = LEVEL_END_DELAY;
	show_message(g, "MISSION FAILED", LEVEL_END_DELAY);
	play_sound_effect(g, "mission_failed");
}

bool	update_end_state(t_game *g)
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
