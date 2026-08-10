/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	hue_color(int hue)
{
	int	section;
	int	rising;
	int	falling;

	section = hue / 60;
	rising = 0x35 + (hue % 60) * 0xB3 / 60;
	falling = 0xE8 - (hue % 60) * 0xB3 / 60;
	if (section == 0)
		return (0xE80000 | rising << 8 | 0x35);
	if (section == 1)
		return (falling << 16 | 0xE800 | 0x35);
	if (section == 2)
		return (0x350000 | 0xE800 | rising);
	if (section == 3)
		return (0x350000 | falling << 8 | 0xE8);
	if (section == 4)
		return (rising << 16 | 0x3500 | 0xE8);
	return (0xE80000 | 0x3500 | falling);
}

int	editor_cell_color(char token)
{
	const char	*tokens;
	char		*position;
	int			index;

	tokens = "10245367vVNX8BPLTHM9QJGWSEKIDCabcdefghijkl_";
	if (token == ' ')
		token = '_';
	position = ft_strchr(tokens, token);
	if (!position)
		return (0x10151C);
	index = position - tokens;
	return (hue_color(index * 137 % 360));
}
