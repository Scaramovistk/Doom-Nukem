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

static void	draw_hud_texture(t_game *g, t_texture *texture, t_coord pos,
		int scale);

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
	draw_hud_texture(g, &g->assets.ammo_icon,
		(t_coord){pos.x + 8, pos.y + 7}, 1);
	draw_number(g, g->hud.ammo, (t_coord){pos.x + 42, pos.y + 10}, 5);
}

static void	draw_inventory(t_game *g)
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
			hud_frame(g, (t_coord){pos.x + i * 52 + 4, pos.y + 4},
				(t_coord){34, 34}, YELLOW);
		draw_hud_texture(g, &g->assets.item_icons[i],
			(t_coord){pos.x + 5 + i * 52, pos.y + 5}, 1);
		draw_number(g, g->hud.inventory[i],
			(t_coord){pos.x + 4 + i * 52, pos.y + 32}, 2);
		i++;
	}
}

static void	draw_crosshair(t_game *g)
{
	t_coord	center;

	center = (t_coord){WIN_WIDTH / 2, WIN_HEIGHT / 2 + (int)g->player.pitch};
	hud_rect(g, (t_coord){center.x - 14, center.y}, (t_coord){10, 2}, WHITE);
	hud_rect(g, (t_coord){center.x + 5, center.y}, (t_coord){10, 2}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y - 14}, (t_coord){2, 10}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y + 5}, (t_coord){2, 10}, WHITE);
	hud_rect(g, (t_coord){center.x, center.y}, (t_coord){2, 2}, RED);
}

static void	draw_score(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){(WIN_WIDTH / 2) - 64, 24};
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

	if (c >= 'a' && c <= 'z')
		c -= 32;
	if (ft_isdigit(c))
		return (draw_digit(g, c - '0', pos, scale));
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

static void	draw_punctuation(t_game *g, char c, t_coord pos, int scale)
{
	if (c == '.' || c == '!' || c == ':')
		hud_rect(g, (t_coord){pos.x + scale, pos.y + 4 * scale},
			(t_coord){scale, scale}, WHITE);
	if (c == '!' || c == ':')
		hud_rect(g, (t_coord){pos.x + scale, pos.y},
			(t_coord){scale, 3 * scale}, WHITE);
	if (c == '-' || c == ':')
		hud_rect(g, (t_coord){pos.x, pos.y + 2 * scale},
			(t_coord){3 * scale, scale}, WHITE);
}

static void	draw_text(t_game *g, const char *text, t_coord pos, int scale)
{
	int	i;

	i = 0;
	while (text[i])
	{
		draw_char(g, text[i], (t_coord){pos.x + i * scale * 4, pos.y}, scale);
		draw_punctuation(g, text[i],
			(t_coord){pos.x + i * scale * 4, pos.y}, scale);
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
	if (width > WIN_WIDTH - 80)
		width = WIN_WIDTH - 80;
	pos = (t_coord){(WIN_WIDTH - width) / 2, 90};
	hud_rect(g, (t_coord){pos.x - 10, pos.y - 10},
		(t_coord){width + 20, scale * 5 + 20}, HUD_BG);
	hud_frame(g, (t_coord){pos.x - 10, pos.y - 10},
		(t_coord){width + 20, scale * 5 + 20}, HUD_BORDER);
	draw_text(g, g->message.text, pos, scale);
}

static void	draw_fps(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){WIN_WIDTH - 136, 24};
	hud_rect(g, pos, (t_coord){112, 42}, HUD_BG);
	hud_frame(g, pos, (t_coord){112, 42}, HUD_BORDER);
	draw_text(g, "FPS", (t_coord){pos.x + 12, pos.y + 12}, 3);
	draw_number(g, g->hud.fps, (t_coord){pos.x + 58, pos.y + 9}, 4);
}

static void	draw_weapon_name(t_game *g, t_coord pos)
{
	if (g->hud.selected_weapon == 1)
		draw_text(g, "BLASTER", (t_coord){pos.x + 22, pos.y + 164}, 2);
	else
		draw_text(g, "PISTOL", (t_coord){pos.x + 34, pos.y + 164}, 2);
}

static void	draw_hud_texture(t_game *g, t_texture *texture, t_coord pos,
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

static t_texture	*weapon_texture(t_game *g)
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

static void	draw_weapon_view(t_game *g)
{
	t_coord	pos;
	int		kick;

	kick = (int)(g->hud.weapon_flash * 60.0);
	pos = (t_coord){WIN_WIDTH / 2 - (32 * HUD_WEAPON_SCALE) / 2,
		WIN_HEIGHT - (32 * HUD_WEAPON_SCALE) + kick};
	draw_hud_texture(g, weapon_texture(g), pos, HUD_WEAPON_SCALE);
	draw_weapon_name(g, pos);
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
}
