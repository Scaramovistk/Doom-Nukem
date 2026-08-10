/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authored_parse_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	parse_block_action(t_authored_action *a, const char *args)
{
	char	block[16];

	a->type = AUTH_ACTION_BLOCK;
	if (sscanf(args, "%d %d %15s", &a->cell.x, &a->cell.y, block) != 3)
		return (false);
	a->value = block_from_name(block);
	return (a->value >= 0);
}

bool	parse_texture_action(t_authored_action *a, const char *args)
{
	a->type = AUTH_ACTION_TEXTURE_SWAP;
	return (sscanf(args, "%d %d", &a->target, &a->value) == 2
		&& a->target >= 0 && a->target < TEXTURES_NB && a->value >= 0
		&& a->value < TEXTURES_NB);
}
