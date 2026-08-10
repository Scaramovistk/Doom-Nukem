/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_number(t_game *g, int value, t_coord pos, int scale)
{
	char	*text;
	int		i;

	if (value < 0)
		value = 0;
	text = ft_itoa(value);
	if (!text)
		return ;
	i = 0;
	while (text[i])
	{
		if (ft_isdigit(text[i]))
			draw_digit(g, text[i] - '0', (t_coord){pos.x + i * scale * 4,
				pos.y}, scale);
		i++;
	}
	free(text);
}

void	draw_health(t_game *g)
{
	int	width;
	int	fill;

	width = 220;
	fill = (width - 8) * g->hud.health / g->hud.max_health;
	hud_rect(g, (t_coord){24, WIN_HEIGHT - 64}, (t_coord){width, 34}, HUD_BG);
	hud_frame(g, (t_coord){24, WIN_HEIGHT - 64}, (t_coord){width, 34},
		HUD_BORDER);
	hud_rect(g, (t_coord){30, WIN_HEIGHT - 58}, (t_coord){fill, 22}, RED);
}

void	draw_ammo(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){WIN_WIDTH - 142, WIN_HEIGHT - 76};
	hud_rect(g, pos, (t_coord){118, 46}, HUD_BG);
	hud_frame(g, pos, (t_coord){118, 46}, HUD_BORDER);
	draw_hud_texture(g, &g->assets.ammo_icon, (t_coord){pos.x + 8, pos.y + 7},
		1);
	draw_number(g, g->hud.magazine[g->hud.selected_weapon], (t_coord){pos.x
		+ 42, pos.y + 10}, 5);
}

void	draw_inventory(t_game *g)
{
	t_coord	pos;
	int		i;

	pos = (t_coord){(WIN_WIDTH / 2) - 102, WIN_HEIGHT - 58};
	i = 0;
	while (i < 4)
	{
		hud_rect(g, (t_coord){pos.x + i * 52, pos.y}, (t_coord){42, 42},
			HUD_BG);
		hud_frame(g, (t_coord){pos.x + i * 52, pos.y}, (t_coord){42, 42},
			HUD_BORDER);
		if (i == g->hud.selected_item)
			hud_frame(g, (t_coord){pos.x + i * 52 + 4, pos.y + 4}, (t_coord){34,
				34}, YELLOW);
		draw_hud_texture(g, &g->assets.item_icons[i], (t_coord){pos.x + 5 + i
			* 52, pos.y + 5}, 1);
		draw_number(g, g->hud.inventory[i], (t_coord){pos.x + 4 + i * 52, pos.y
			+ 32}, 2);
		i++;
	}
}

void	draw_crosshair(t_game *g)
{
	t_coord	center;

	center = (t_coord){WIN_WIDTH / 2, WIN_HEIGHT / 2 + (int)g->player.pitch};
	hud_rect(g, (t_coord){center.x - 14, center.y}, (t_coord){10, 2}, WHITE);
	hud_rect(g, (t_coord){center.x + 5, center.y}, (t_coord){10, 2}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y - 14}, (t_coord){2, 10}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y + 5}, (t_coord){2, 10}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y}, (t_coord){2, 2}, RED);
}
