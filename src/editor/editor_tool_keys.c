/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_keys.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static char	page_zero_tool(int key)
{
	const int	keys[12] = {KEY_1, KEY_0, KEY_2, KEY_4, KEY_5, KEY_3,
		KEY_6, KEY_7, KEY_SEMICOLON, KEY_V, KEY_N, KEY_X};
	const char	tools[13] = "10245367vVNX";
	int			i;

	i = 0;
	while (i < 12)
	{
		if (key == keys[i])
			return (tools[i]);
		i++;
	}
	return ('\0');
}

static char	page_one_tool(int key)
{
	const int	keys[12] = {KEY_8, KEY_B, KEY_P, KEY_L, KEY_T, KEY_H,
		KEY_M, KEY_9, KEY_Q, KEY_J, KEY_G, KEY_W};
	const char	tools[13] = "8BPLTHM9QJGW";
	int			i;

	i = 0;
	while (i < 12)
	{
		if (key == keys[i])
			return (tools[i]);
		i++;
	}
	return ('\0');
}

static char	page_two_tool(int key)
{
	const int	keys[12] = {KEY_S, KEY_E, KEY_K, KEY_I, KEY_D, KEY_C,
		KEY_A, KEY_F, KEY_O, KEY_R, KEY_U, KEY_Y};
	const char	tools[13] = "SEKIDCabcdef";
	int			i;

	i = 0;
	while (i < 12)
	{
		if (key == keys[i])
			return (tools[i]);
		i++;
	}
	return ('\0');
}

static char	page_three_tool(int key)
{
	const int	keys[7] = {KEY_Z, KEY_MINUS, KEY_EQUAL, KEY_COMMA,
		KEY_PERIOD, KEY_SLASH, KEY_APOSTROPHE};
	const char	tools[8] = "ghijkl_";
	int			i;

	i = 0;
	while (i < 7)
	{
		if (key == keys[i])
			return (tools[i]);
		i++;
	}
	return ('\0');
}

char	editor_tool_from_key(int key)
{
	char	tool;

	tool = page_zero_tool(key);
	if (!tool)
		tool = page_one_tool(key);
	if (!tool)
		tool = page_two_tool(key);
	if (!tool)
		tool = page_three_tool(key);
	return (tool);
}
