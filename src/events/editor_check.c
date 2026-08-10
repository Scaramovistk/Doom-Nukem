/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	print_level_stats(t_game *g, char *path)
{
	int	i;
	int	blocking;

	printf("Level OK: %s\n", path);
	printf("  map: %dx%d\n", g->map.width, g->map.height);
	printf("  sectors: %d\n", g->map.sector_count);
	printf("  angled walls: %d\n", g->map.segment_count);
	printf("  sprites: %d\n", g->map.sprite_count);
	blocking = 0;
	i = 0;
	while (i < g->map.object_count)
		blocking += g->map.objects[i++].blocks_passage;
	printf("  world objects: %d (%d blocking)\n", g->map.object_count,
		blocking);
	printf("  authored actions: %d\n", g->map.action_count);
	printf("  items: %d\n", g->map.item_count);
	printf("  elevator panels: %d\n", g->map.elevator_count);
	printf("  secret doors: %d\n", g->map.secret_count);
	printf("  locked doors: %d\n", g->map.locked_door_count);
}

int	check_level_file(char *path)
{
	t_game	g;
	char	*argv[2];

	init_game_struct(&g);
	argv[0] = "doom-nukem";
	argv[1] = path;
	if (!ft_parse_file(2, argv, &g))
	{
		free_all(&g);
		return (EXIT_FAILURE);
	}
	print_level_stats(&g, path);
	check_segment_loops(&g);
	free_all(&g);
	return (EXIT_SUCCESS);
}
