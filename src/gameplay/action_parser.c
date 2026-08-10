/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/05 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	block_from_name(const char *name)
{
	if (!ft_strcmp((char *)name, "EMPTY"))
		return (EMPTY);
	if (!ft_strcmp((char *)name, "WALL"))
		return (WALL);
	if (!ft_strcmp((char *)name, "DOOR"))
		return (DOOR);
	if (!ft_strcmp((char *)name, "GLASS"))
		return (TRANSPARENT_WALL);
	if (!ft_strcmp((char *)name, "DECAL"))
		return (DECAL_WALL);
	return (-1);
}

bool	parse_sector_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "FLOOR"))
		a->type = AUTH_ACTION_FLOOR;
	else if (!ft_strcmp(kind, "CEILING"))
		a->type = AUTH_ACTION_CEILING;
	else if (!ft_strcmp(kind, "LIGHT"))
		a->type = AUTH_ACTION_LIGHT;
	else
		return (false);
	if (a->type == AUTH_ACTION_LIGHT)
		return (sscanf(args, "%d %d", &a->target, &a->value) == 2
			&& a->value >= 0 && a->value <= 255);
	return (sscanf(args, "%d %lf", &a->target, &a->values[0]) == 2
		&& isfinite(a->values[0]));
}

bool	parse_object_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "OBJECT_MOVE"))
	{
		a->type = AUTH_ACTION_OBJECT_MOVE;
		return (sscanf(args, "%d %lf %lf", &a->target, &a->values[0],
				&a->values[1]) == 3 && isfinite(a->values[0])
			&& isfinite(a->values[1]));
	}
	if (!ft_strcmp(kind, "OBJECT_BLOCK"))
	{
		a->type = AUTH_ACTION_OBJECT_BLOCK;
		return (sscanf(args, "%d %d", &a->target, &a->value) == 2
			&& (a->value == 0 || a->value == 1));
	}
	if (!ft_strcmp(kind, "OBJECT_SCALE"))
	{
		a->type = AUTH_ACTION_OBJECT_SCALE;
		return (sscanf(args, "%d %lf", &a->target, &a->values[0]) == 2
			&& isfinite(a->values[0]) && a->values[0] > 0.0);
	}
	if (ft_strcmp(kind, "OBJECT_TEXTURE"))
		return (false);
	a->type = AUTH_ACTION_OBJECT_TEXTURE;
	return (sscanf(args, "%d %d", &a->target, &a->value) == 2 && a->value >= 0
		&& a->value < TEXTURES_NB);
}

bool	parse_wall_action(t_authored_action *a, char *kind, char *args)
{
	if (!ft_strcmp(kind, "WALL_MOVE"))
	{
		a->type = AUTH_ACTION_WALL_MOVE;
		return (sscanf(args, "%d %lf %lf %lf %lf", &a->target, &a->values[0],
				&a->values[1], &a->values[2], &a->values[3]) == 5
			&& isfinite(a->values[0]) && isfinite(a->values[1])
			&& isfinite(a->values[2]) && isfinite(a->values[3]));
	}
	if (ft_strcmp(kind, "WALL_TEXTURE"))
		return (false);
	a->type = AUTH_ACTION_WALL_TEXTURE;
	return (sscanf(args, "%d %d", &a->target, &a->value) == 2 && a->value >= 0
		&& a->value < TEXTURES_NB);
}

bool	parse_authored_action(const char *line, t_authored_action *a)
{
	char	kind[24];
	int		offset;

	ft_bzero(a, sizeof(*a));
	offset = 0;
	if (sscanf(line, "ACTION %d %d %lf %23s %n", &a->trigger.x, &a->trigger.y,
			&a->delay, kind, &offset) != 4 || !isfinite(a->delay)
		|| a->delay < 0.0)
		return (false);
	if (!ft_strcmp(kind, "BLOCK"))
		return (parse_block_action(a, line + offset));
	if (!ft_strcmp(kind, "TEXTURE_SWAP"))
		return (parse_texture_action(a, line + offset));
	if (parse_sector_action(a, kind, (char *)line + offset))
		return (true);
	if (parse_object_action(a, kind, (char *)line + offset))
		return (true);
	return (parse_wall_action(a, kind, (char *)line + offset));
}
