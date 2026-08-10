/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static const char	*difficulty_name(int difficulty)
{
	if (difficulty == 0)
		return ("EASY");
	if (difficulty == 2)
		return ("HARD");
	return ("NORMAL");
}

static char	*level_basename(char *path)
{
	char	*slash;

	slash = ft_strrchr(path, '/');
	if (!slash)
		return (path);
	return (slash + 1);
}

static char	*menu_entry_label(t_menu *menu, int i)
{
	char	*name;

	name = level_basename(menu->levels[i]);
	if (i == 0 && ft_strcmp(name, "e1m1.dnk") == 0)
		return ("New Game (Doom E1M1-E1M5)");
	if (ft_strcmp(name, "ctf_1.dnk") == 0)
		return ("Capture the Flag (3 maps)");
	return (name);
}

static void	render_entries(t_game *g)
{
	char	line[LINE_SIZE];
	int		color;
	int		i;

	i = 0;
	while (i < g->menu.level_count)
	{
		color = WHITE;
		ft_strlcpy(line, "  ", LINE_SIZE);
		if (i == g->menu.selected)
		{
			color = ORANGE;
			ft_strlcpy(line, "> ", LINE_SIZE);
		}
		ft_strlcat(line, menu_entry_label(&g->menu, i), LINE_SIZE);
		mlx_string_put(g->mlx, g->mlx_win, 470, 230 + i * 28, color, line);
		i++;
	}
}

void	render_menu(t_game *g)
{
	char	line[LINE_SIZE];

	mlx_clear_window(g->mlx, g->mlx_win);
	mlx_string_put(g->mlx, g->mlx_win, 480, 130, YELLOW, "DOOM-NUKEM");
	mlx_string_put(g->mlx, g->mlx_win, 430, 170, WHITE,
		"Select level with arrows, Enter to start");
	render_entries(g);
	if (!g->menu.level_count)
		mlx_string_put(g->mlx, g->mlx_win, 470, 230, RED,
			"No .cub or .dnk files found in tests/maps");
	ft_strlcpy(line, "Difficulty: ", LINE_SIZE);
	ft_strlcat(line, difficulty_name(g->menu.difficulty), LINE_SIZE);
	mlx_string_put(g->mlx, g->mlx_win, 500, 690, GREEN, line);
	mlx_string_put(g->mlx, g->mlx_win, 430, 720, GREY,
		"Left/Right changes difficulty");
}
