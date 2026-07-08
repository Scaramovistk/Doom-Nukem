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

#include "../include/cub3d.h"

int	main(int argc, char *argv[])
{
	t_game	g;

	init_game_struct(&g);
	if (argc == 1)
	{
		init_menu(&g);
		setup_hooks(&g);
		start_game(&g);
		return (EXIT_SUCCESS);
	}
	if (argc != 2 || !ft_parse_file(argc, argv, &g))
		return (ft_parsing_error(PARSINGP, 0));
	load_game(&g);
	setup_hooks(&g);
	start_game(&g);
	return (EXIT_SUCCESS);
}
