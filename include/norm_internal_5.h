/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_5.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_5_H
# define NORM_INTERNAL_5_H

void		hud_rect(t_game *g, t_coord pos, t_coord size, int color);
void		hud_frame(t_game *g, t_coord pos, t_coord size, int color);
const char	*digit_pattern(int digit);
void		draw_digit(t_game *g, int digit, t_coord pos, int scale);
void		draw_number(t_game *g, int value, t_coord pos, int scale);
void		draw_health(t_game *g);
void		draw_ammo(t_game *g);
void		draw_inventory(t_game *g);
void		draw_crosshair(t_game *g);
void		draw_score(t_game *g);
const char	*letter_pattern(char c);
void		draw_char(t_game *g, char c, t_coord pos, int scale);
void		draw_punctuation(t_game *g, char c, t_coord pos, int scale);
void		draw_text(t_game *g, const char *text, t_coord pos, int scale);
void		draw_message(t_game *g);
void		draw_fps(t_game *g);
void		draw_weapon_name(t_game *g, t_coord pos);
void		draw_hud_texture(t_game *g, t_texture *texture, t_coord pos,
				int scale);
t_texture	*weapon_texture(t_game *g);
void		draw_weapon_view(t_game *g);
const char	*story_line(int level, bool debrief, int line);
const char	*briefing_text(int level, int line);
const char	*debriefing_text(int level, int line);
void		draw_story_line(t_game *g, const char *line, int y);
void		draw_campaign_story(t_game *g);
void		sort_sprites(t_sprite_draw *sprites, int count);
void		init_sprite_draw(t_sprite_draw *s, t_position pos, int index,
				t_game *g);

#endif
