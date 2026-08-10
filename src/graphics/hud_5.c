/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_weapon_view(t_game *g)
{
	t_coord	pos;
	int		kick;

	kick = (int)(g->hud.weapon_flash * 60.0);
	pos = (t_coord){WIN_WIDTH / 2 - (32 * HUD_WEAPON_SCALE) / 2, WIN_HEIGHT
		- (32 * HUD_WEAPON_SCALE) + kick};
	draw_hud_texture(g, weapon_texture(g), pos, HUD_WEAPON_SCALE);
	draw_weapon_name(g, pos);
}

const char	*story_line(int level, bool debrief, int line)
{
	if (level < 1 || level > 5 || line < 0 || line >= 4)
		return ("");
	if (debrief)
		return (debriefing_text(level - 1, line));
	return (briefing_text(level - 1, line));
}

void	draw_story_line(t_game *g, const char *line, int y)
{
	int	width;

	width = (int)ft_strlen(line) * 12;
	draw_text(g, line, (t_coord){(WIN_WIDTH - width) / 2, y}, 3);
}

void	draw_campaign_story(t_game *g)
{
	int	line;

	hud_rect(g, (t_coord){0, 0}, (t_coord){WIN_WIDTH, WIN_HEIGHT}, BLACK);
	hud_rect(g, (t_coord){120, 140}, (t_coord){WIN_WIDTH - 240, 440}, HUD_BG);
	hud_frame(g, (t_coord){120, 140}, (t_coord){WIN_WIDTH - 240, 440}, YELLOW);
	if (g->story_is_debrief)
		draw_story_line(g, "MISSION DEBRIEF", 180);
	else
		draw_story_line(g, "MISSION BRIEFING", 180);
	line = 0;
	while (line < 4)
	{
		draw_story_line(g, story_line(g->campaign_level, g->story_is_debrief,
				line), 270 + line * 52);
		line++;
	}
	draw_story_line(g, "PRESS ENTER TO CONTINUE", 520);
}

void	draw_hud(t_game *g)
{
	draw_crosshair(g);
	draw_weapon_view(g);
	draw_health(g);
	draw_ammo(g);
	draw_inventory(g);
	draw_score(g);
	draw_fps(g);
	draw_message(g);
	if (g->story_visible)
		draw_campaign_story(g);
}
