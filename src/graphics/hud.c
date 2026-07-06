/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	hud_pixel(t_game *g, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	put_pixel(&g->img, x, y, color);
}

static void	hud_rect(t_game *g, t_coord pos, t_coord size, int color)
{
	int	x;
	int	y;

	y = pos.y;
	while (y < pos.y + size.y)
	{
		x = pos.x;
		while (x < pos.x + size.x)
			hud_pixel(g, x++, y, color);
		y++;
	}
}

static void	hud_frame(t_game *g, t_coord pos, t_coord size, int color)
{
	hud_rect(g, pos, (t_coord){size.x, 2}, color);
	hud_rect(g, (t_coord){pos.x, pos.y + size.y - 2},
		(t_coord){size.x, 2}, color);
	hud_rect(g, pos, (t_coord){2, size.y}, color);
	hud_rect(g, (t_coord){pos.x + size.x - 2, pos.y},
		(t_coord){2, size.y}, color);
}

static const char	*digit_pattern(int digit)
{
	static const char	*patterns[10] = {
		"111101101101111", "010110010010111", "111001111100111",
		"111001111001111", "101101111001001", "111100111001111",
		"111100111101111", "111001001001001", "111101111101111",
		"111101111001111"
	};

	if (digit < 0 || digit > 9)
		return (patterns[0]);
	return (patterns[digit]);
}

static void	draw_digit(t_game *g, int digit, t_coord pos, int scale)
{
	const char	*pattern;
	t_coord		cell;

	pattern = digit_pattern(digit);
	cell.y = 0;
	while (cell.y < 5)
	{
		cell.x = 0;
		while (cell.x < 3)
		{
			if (pattern[cell.y * 3 + cell.x] == '1')
				hud_rect(g, (t_coord){pos.x + cell.x * scale,
					pos.y + cell.y * scale}, (t_coord){scale, scale}, WHITE);
			cell.x++;
		}
		cell.y++;
	}
}

static void	draw_number(t_game *g, int value, t_coord pos, int scale)
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
			draw_digit(g, text[i] - '0',
				(t_coord){pos.x + i * scale * 4, pos.y}, scale);
		i++;
	}
	free(text);
}

static void	draw_health(t_game *g)
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

static void	draw_ammo(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){WIN_WIDTH - 142, WIN_HEIGHT - 76};
	hud_rect(g, pos, (t_coord){118, 46}, HUD_BG);
	hud_frame(g, pos, (t_coord){118, 46}, HUD_BORDER);
	hud_rect(g, (t_coord){pos.x + 12, pos.y + 14}, (t_coord){18, 18}, ORANGE);
	draw_number(g, g->hud.ammo, (t_coord){pos.x + 42, pos.y + 10}, 5);
}

static void	draw_inventory(t_game *g)
{
	t_coord	pos;
	int		i;
	int		color;

	pos = (t_coord){(WIN_WIDTH / 2) - 102, WIN_HEIGHT - 58};
	i = 0;
	while (i < 4)
	{
		color = DARK_GREY;
		if (g->hud.inventory[i])
			color = BLUE;
		hud_rect(g, (t_coord){pos.x + i * 52, pos.y}, (t_coord){42, 42},
			HUD_BG);
		hud_frame(g, (t_coord){pos.x + i * 52, pos.y}, (t_coord){42, 42},
			HUD_BORDER);
		hud_rect(g, (t_coord){pos.x + 12 + i * 52, pos.y + 12},
			(t_coord){18, 18}, color);
		draw_number(g, g->hud.inventory[i],
			(t_coord){pos.x + 4 + i * 52, pos.y + 32}, 2);
		i++;
	}
}

static void	draw_score(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){24, 24};
	hud_rect(g, pos, (t_coord){128, 42}, HUD_BG);
	hud_frame(g, pos, (t_coord){128, 42}, HUD_BORDER);
	hud_rect(g, (t_coord){pos.x + 12, pos.y + 12}, (t_coord){18, 18}, YELLOW);
	draw_number(g, g->hud.score, (t_coord){pos.x + 42, pos.y + 9}, 4);
}

static const char	*letter_pattern(char c)
{
	static const char	*patterns[26] = {
		"010101111101101", "110101110101110", "011100100100011",
		"110101101101110", "111100110100111", "111100110100100",
		"011100101101011", "101101111101101", "111010010010111",
		"001001001101011", "101101110101101", "100100100100111",
		"101111111101101", "101111111111101", "010101101101010",
		"110101110100100", "010101101111011", "110101110101101",
		"011100010001110", "111010010010010", "101101101101011",
		"101101101010010", "101101111111101", "101101010101101",
		"101101010010010", "111001010100111"
	};

	if (c < 'A' || c > 'Z')
		return (NULL);
	return (patterns[c - 'A']);
}

static void	draw_char(t_game *g, char c, t_coord pos, int scale)
{
	const char	*pattern;
	t_coord		cell;

	pattern = letter_pattern(c);
	if (!pattern)
		return ;
	cell.y = 0;
	while (cell.y < 5)
	{
		cell.x = 0;
		while (cell.x < 3)
		{
			if (pattern[cell.y * 3 + cell.x] == '1')
				hud_rect(g, (t_coord){pos.x + cell.x * scale,
					pos.y + cell.y * scale}, (t_coord){scale, scale}, WHITE);
			cell.x++;
		}
		cell.y++;
	}
}

static void	draw_text(t_game *g, const char *text, t_coord pos, int scale)
{
	int	i;

	i = 0;
	while (text[i])
	{
		draw_char(g, text[i], (t_coord){pos.x + i * scale * 4, pos.y}, scale);
		i++;
	}
}

static void	draw_message(t_game *g)
{
	t_coord	pos;
	int		scale;
	int		width;

	if (g->message.timer <= 0.0)
		return ;
	scale = 3;
	width = (int)ft_strlen(g->message.text) * scale * 4;
	pos = (t_coord){(WIN_WIDTH - width) / 2, 90};
	hud_rect(g, (t_coord){pos.x - 10, pos.y - 10},
		(t_coord){width + 20, scale * 5 + 20}, HUD_BG);
	hud_frame(g, (t_coord){pos.x - 10, pos.y - 10},
		(t_coord){width + 20, scale * 5 + 20}, HUD_BORDER);
	draw_text(g, g->message.text, pos, scale);
}

void	draw_hud(t_game *g)
{
	draw_health(g);
	draw_ammo(g);
	draw_inventory(g);
	draw_score(g);
	draw_message(g);
}
