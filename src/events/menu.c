/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_cub_file(const char *name)
{
	int	len;

	len = ft_strlen(name);
	return (len > 4 && ft_strcmp((char *)name + len - 4, ".cub") == 0);
}

static void	add_level(t_menu *menu, const char *name)
{
	if (menu->level_count >= MENU_MAX_LEVELS)
		return ;
	ft_strlcpy(menu->levels[menu->level_count], MENU_LEVEL_DIR, LINE_SIZE);
	ft_strlcat(menu->levels[menu->level_count], "/", LINE_SIZE);
	ft_strlcat(menu->levels[menu->level_count], name, LINE_SIZE);
	menu->level_count++;
}

static void	load_menu_levels(t_menu *menu)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(MENU_LEVEL_DIR);
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (is_cub_file(entry->d_name))
			add_level(menu, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
}

void	init_menu(t_game *g)
{
	g->state = STATE_MENU;
	g->menu.active = true;
	load_menu_levels(&g->menu);
	g->mlx = mlx_init();
	g->mlx_win = mlx_new_window(g->mlx, WIN_WIDTH, WIN_HEIGHT, "Doom-Nukem");
	if (!g->mlx || !g->mlx_win)
		error("Unable to create menu window", g);
}

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

void	render_menu(t_game *g)
{
	int		i;
	int		y;
	int		color;
	char	line[LINE_SIZE];

	mlx_clear_window(g->mlx, g->mlx_win);
	mlx_string_put(g->mlx, g->mlx_win, 480, 130, YELLOW, "DOOM-NUKEM");
	mlx_string_put(g->mlx, g->mlx_win, 430, 170, WHITE,
		"Select level with arrows, Enter to start");
	i = 0;
	y = 230;
	while (i < g->menu.level_count)
	{
		color = WHITE;
		if (i == g->menu.selected)
			color = ORANGE;
		ft_strlcpy(line, "  ", LINE_SIZE);
		if (i == g->menu.selected)
			ft_strlcpy(line, "> ", LINE_SIZE);
		ft_strlcat(line, level_basename(g->menu.levels[i]), LINE_SIZE);
		mlx_string_put(g->mlx, g->mlx_win, 470, y + i * 28, color, line);
		i++;
	}
	if (!g->menu.level_count)
		mlx_string_put(g->mlx, g->mlx_win, 470, y, RED,
			"No .cub files found in tests/maps");
	ft_strlcpy(line, "Difficulty: ", LINE_SIZE);
	ft_strlcat(line, difficulty_name(g->menu.difficulty), LINE_SIZE);
	mlx_string_put(g->mlx, g->mlx_win, 500, 690, GREEN, line);
	mlx_string_put(g->mlx, g->mlx_win, 430, 720, GREY,
		"Left/Right changes difficulty");
}

static void	start_selected_level(t_game *g)
{
	char	*argv[2];

	if (!g->menu.level_count)
		return ;
	argv[0] = "doom-nukem";
	argv[1] = g->menu.levels[g->menu.selected];
	if (!ft_parse_file(2, argv, g))
	{
		show_message(g, "LEVEL LOAD FAILED", MESSAGE_DISPLAY_TIME);
		render_menu(g);
		return ;
	}
	g->state = STATE_PLAYING;
	g->menu.active = false;
	load_game(g);
	if (g->menu.difficulty == 0)
	{
		g->hud.health = 150;
		g->hud.max_health = 150;
		g->hud.ammo += 20;
	}
	else if (g->menu.difficulty == 2)
	{
		g->hud.health = 75;
		g->hud.max_health = 75;
		if (g->hud.ammo > 10)
			g->hud.ammo -= 10;
	}
	setup_hooks(g);
	start_level_flow(g);
	start_background_music(g);
	render(g);
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
