/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_message(t_game *g)
{
	t_coord	pos;
	int		scale;
	int		width;

	if (g->message.timer <= 0.0)
		return ;
	scale = 3;
	width = (int)ft_strlen(g->message.text) * scale * 4;
	if (width > WIN_WIDTH - 80)
		width = WIN_WIDTH - 80;
	pos = (t_coord){(WIN_WIDTH - width) / 2, 90};
	hud_rect(g, (t_coord){pos.x - 10, pos.y - 10}, (t_coord){width + 20, scale
		* 5 + 20}, HUD_BG);
	hud_frame(g, (t_coord){pos.x - 10, pos.y - 10}, (t_coord){width + 20, scale
		* 5 + 20}, HUD_BORDER);
	draw_text(g, g->message.text, pos, scale);
}

void	draw_fps(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){WIN_WIDTH - 136, 24};
	hud_rect(g, pos, (t_coord){112, 42}, HUD_BG);
	hud_frame(g, pos, (t_coord){112, 42}, HUD_BORDER);
	draw_text(g, "FPS", (t_coord){pos.x + 12, pos.y + 12}, 3);
	draw_number(g, g->hud.fps, (t_coord){pos.x + 58, pos.y + 9}, 4);
}

void	draw_weapon_name(t_game *g, t_coord pos)
{
	if (g->hud.selected_weapon == 1)
		draw_text(g, "BLASTER", (t_coord){pos.x + 22, pos.y + 164}, 2);
	else
		draw_text(g, "PISTOL", (t_coord){pos.x + 34, pos.y + 164}, 2);
}

void	draw_hud_texture(t_game *g, t_texture *texture, t_coord pos,
		int scale)
{
	t_coord	src;
	t_coord	dst;
	int		color;

	if (!texture->img.ptr)
		return ;
	src.y = 0;
	while (src.y < 32)
	{
		src.x = 0;
		while (src.x < 32)
		{
			color = get_pixel(&texture->img, src.x, src.y);
			if ((color & 0x00FFFFFF) != 0x00FF00FF)
			{
				dst.x = pos.x + src.x * scale;
				dst.y = pos.y + src.y * scale;
				hud_rect(g, dst, (t_coord){scale, scale}, color);
			}
			src.x++;
		}
		src.y++;
	}
}

t_texture	*weapon_texture(t_game *g)
{
	int	weapon;
	int	state;

	weapon = g->hud.selected_weapon;
	if (weapon < 0 || weapon >= WEAPON_NB)
		weapon = 0;
	state = 0;
	if (g->hud.weapon_flash > 0.0)
		state = 1;
	return (&g->assets.hud_weapons[weapon][state]);
}
