/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_level_scan.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_level_file(const char *name)
{
	int	len;

	len = ft_strlen(name);
	return (len > 4 && (ft_strcmp((char *)name + len - 4, ".cub") == 0
			|| ft_strcmp((char *)name + len - 4, ".dnk") == 0));
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

static bool	file_exists(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

void	menu_load_levels(t_menu *menu)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(MENU_LEVEL_DIR);
	if (!dir)
		return ;
	if (file_exists(MENU_LEVEL_DIR "/e1m1.dnk"))
		add_level(menu, "e1m1.dnk");
	if (file_exists(MENU_LEVEL_DIR "/ctf_1.dnk"))
		add_level(menu, "ctf_1.dnk");
	entry = readdir(dir);
	while (entry)
	{
		if (is_level_file(entry->d_name)
			&& ft_strcmp(entry->d_name, "e1m1.dnk") != 0
			&& ft_strncmp(entry->d_name, "ctf_", 4) != 0)
			add_level(menu, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
}
