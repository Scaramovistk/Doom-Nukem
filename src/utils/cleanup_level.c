/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_level.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_unpack_dir(t_game *g)
{
	if (!g->unpacked_level || !g->unpack_dir[0])
		return (false);
	return (ft_strncmp(g->unpack_dir, DNK_UNPACK_ROOT,
			ft_strlen(DNK_UNPACK_ROOT)) == 0);
}

static void	remove_tree(char *path);

static void	remove_entry(char *path, struct dirent *entry)
{
	char		child[LINE_SIZE];
	struct stat	st;

	if (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, ".."))
		return ;
	snprintf(child, LINE_SIZE, "%s/%s", path, entry->d_name);
	if (!stat(child, &st) && S_ISDIR(st.st_mode))
		remove_tree(child);
	else
		unlink(child);
}

static void	remove_tree(char *path)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(path);
	if (!dir)
		return ((void)unlink(path));
	entry = readdir(dir);
	while (entry)
	{
		remove_entry(path, entry);
		entry = readdir(dir);
	}
	closedir(dir);
	rmdir(path);
}

void	cleanup_unpacked_level(t_game *g)
{
	if (is_unpack_dir(g))
		remove_tree(g->unpack_dir);
}
