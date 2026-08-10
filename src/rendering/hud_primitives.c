/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_primitives.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	hud_pixel(t_game *g, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	put_pixel(&g->img, x, y, color);
}

void	hud_rect(t_game *g, t_coord pos, t_coord size, int color)
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

void	hud_frame(t_game *g, t_coord pos, t_coord size, int color)
{
	hud_rect(g, pos, (t_coord){size.x, 2}, color);
	hud_rect(g, (t_coord){pos.x, pos.y + size.y - 2}, (t_coord){size.x, 2},
		color);
	hud_rect(g, pos, (t_coord){2, size.y}, color);
	hud_rect(g, (t_coord){pos.x + size.x - 2, pos.y}, (t_coord){2, size.y},
		color);
}

const char	*digit_pattern(int digit)
{
	const char *const	patterns[10] = {"111101101101111", "010110010010111",
		"111001111100111", "111001111001111", "101101111001001",
		"111100111001111", "111100111101111", "111001001001001",
		"111101111101111", "111101111001111"};

	if (digit < 0 || digit > 9)
		return (patterns[0]);
	return (patterns[digit]);
}

void	draw_digit(t_game *g, int digit, t_coord pos, int scale)
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
				hud_rect(g, (t_coord){pos.x + cell.x * scale, pos.y + cell.y
					* scale}, (t_coord){scale, scale}, WHITE);
			cell.x++;
		}
		cell.y++;
	}
}
