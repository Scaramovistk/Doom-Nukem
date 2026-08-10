/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_text.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_score(t_game *g)
{
	t_coord	pos;

	pos = (t_coord){(WIN_WIDTH / 2) - 64, 24};
	hud_rect(g, pos, (t_coord){128, 42}, HUD_BG);
	hud_frame(g, pos, (t_coord){128, 42}, HUD_BORDER);
	hud_rect(g, (t_coord){pos.x + 12, pos.y + 12}, (t_coord){18, 18}, YELLOW);
	draw_number(g, g->hud.score, (t_coord){pos.x + 42, pos.y + 9}, 4);
}

const char	*letter_pattern(char c)
{
	const char *const	patterns[26] = {"010101111101101", "110101110101110",
		"011100100100011", "110101101101110", "111100110100111",
		"111100110100100", "011100101101011", "101101111101101",
		"111010010010111", "001001001101011", "101101110101101",
		"100100100100111", "101111111101101", "101111111111101",
		"010101101101010", "110101110100100", "010101101111011",
		"110101110101101", "011100010001110", "111010010010010",
		"101101101101011", "101101101010010", "101101111111101",
		"101101010101101", "101101010010010", "111001010100111"};

	if (c < 'A' || c > 'Z')
		return (NULL);
	return (patterns[c - 'A']);
}

void	draw_char(t_game *g, char c, t_coord pos, int scale)
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
				hud_rect(g, (t_coord){pos.x + cell.x * scale, pos.y + cell.y
					* scale}, (t_coord){scale, scale}, WHITE);
			cell.x++;
		}
		cell.y++;
	}
}

void	draw_punctuation(t_game *g, char c, t_coord pos, int scale)
{
	if (c == '.' || c == '!' || c == ':')
		hud_rect(g, (t_coord){pos.x + scale, pos.y + 4 * scale},
			(t_coord){scale, scale}, WHITE);
	if (c == '!' || c == ':')
		hud_rect(g, (t_coord){pos.x + scale, pos.y}, (t_coord){scale, 3
			* scale}, WHITE);
	if (c == '-' || c == ':')
		hud_rect(g, (t_coord){pos.x, pos.y + 2 * scale}, (t_coord){3 * scale,
			scale}, WHITE);
}

void	draw_text(t_game *g, const char *text, t_coord pos, int scale)
{
	int	i;

	i = 0;
	while (text[i])
	{
		draw_char(g, text[i], (t_coord){pos.x + i * scale * 4, pos.y}, scale);
		draw_punctuation(g, text[i], (t_coord){pos.x + i * scale * 4, pos.y},
			scale);
		i++;
	}
}
