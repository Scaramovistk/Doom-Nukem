/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:19:32 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:19:36 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	run_level(int argc, char *argv[], t_game *g)
{
	if (argc == 1)
	{
		init_menu(g);
		setup_hooks(g);
		start_game(g);
		return (EXIT_SUCCESS);
	}
	if (argc != 2 || !ft_parse_file(argc, argv, g))
	{
		free_all(g);
		return (ft_parsing_error(PARSINGP, EXIT_FAILURE));
	}
	compute_sector_origins(g);
	init_bsp_visibility(g);
	load_game(g);
	setup_hooks(g);
	start_game(g);
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_game	g;

	if (argc >= 2 && (!ft_strcmp(argv[1], "--edit") || !ft_strcmp(argv[1],
				"--pack")))
		return (run_editor(argc, argv));
	if (argc >= 2 && !ft_strcmp(argv[1], "--check"))
	{
		if (argc != 3)
			return (ft_parsing_error(ARGSP, EXIT_FAILURE));
		return (check_level_file(argv[2]));
	}
	init_game_struct(&g);
	return (run_level(argc, argv, &g));
}
