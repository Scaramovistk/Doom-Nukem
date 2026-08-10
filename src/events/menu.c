/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	init_menu(t_game *g)
{
	g->state = STATE_MENU;
	g->menu.active = true;
	menu_load_levels(&g->menu);
	g->mlx = mlx_init();
	g->mlx_win = mlx_new_window(g->mlx, WIN_WIDTH, WIN_HEIGHT, "Doom-Nukem");
	if (!g->mlx || !g->mlx_win)
		error("Unable to create menu window", g);
}

static bool	new_game_selected(t_menu *menu)
{
	char	*name;

	name = ft_strrchr(menu->levels[0], '/');
	if (name)
		name++;
	else
		name = menu->levels[0];
	return (menu->selected == 0 && ft_strcmp(name, "e1m1.dnk") == 0);
}

static void	start_selected_level(t_game *g)
{
	if (!g->menu.level_count)
		return ;
	g->campaign_mode = new_game_selected(&g->menu);
	g->story_visible = false;
	g->story_is_debrief = false;
	if (!load_level_path(g, g->menu.levels[g->menu.selected]))
	{
		show_message(g, "LEVEL LOAD FAILED", MESSAGE_DISPLAY_TIME);
		render_menu(g);
	}
}

int	menu_key(int key, t_game *g)
{
	if (key == KEY_UP && g->menu.selected > 0)
		g->menu.selected--;
	else if (key == KEY_DOWN && g->menu.selected < g->menu.level_count - 1)
		g->menu.selected++;
	else if (key == KEY_LEFT && g->menu.difficulty > 0)
		g->menu.difficulty--;
	else if (key == KEY_RIGHT && g->menu.difficulty < MENU_DIFFICULTIES - 1)
		g->menu.difficulty++;
	else if (key == KEY_ENTER)
	{
		start_selected_level(g);
		return (0);
	}
	render_menu(g);
	return (0);
}
