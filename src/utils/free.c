/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:50:21 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:50:35 by rperez-t         ###   ########.fr       */
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

static void	remove_tree(char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			child[LINE_SIZE];
	struct stat		st;

	dir = opendir(path);
	if (!dir)
	{
		unlink(path);
		return ;
	}
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, ".") && ft_strcmp(entry->d_name, ".."))
		{
			snprintf(child, LINE_SIZE, "%s/%s", path, entry->d_name);
			if (!stat(child, &st) && S_ISDIR(st.st_mode))
				remove_tree(child);
			else
				unlink(child);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	rmdir(path);
}

static void	cleanup_unpacked_level(t_game *g)
{
	if (is_unpack_dir(g))
		remove_tree(g->unpack_dir);
}

void	ft_destroy_textures(t_game *g)
{
	int		i;
	int		j;
	t_img	*img;

	i = TEXTURES_NB;
	while (i--)
	{
		img = &g->assets.textures[i].img;
		if (img->ptr)
			mlx_destroy_image(g->mlx, img->ptr);
	}
	img = &g->assets.floor_texture.img;
	if (img->ptr)
		mlx_destroy_image(g->mlx, img->ptr);
	img = &g->assets.ceiling_texture.img;
	if (img->ptr)
		mlx_destroy_image(g->mlx, img->ptr);
	img = &g->assets.sky_texture.img;
	if (img->ptr)
		mlx_destroy_image(g->mlx, img->ptr);
	i = SPRITE_FRAME_NB;
	while (i--)
	{
		img = &g->assets.sprite_frames[i].img;
		if (img->ptr)
			mlx_destroy_image(g->mlx, img->ptr);
	}
	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
		{
			img = &g->assets.hud_weapons[i][j].img;
			if (img->ptr)
				mlx_destroy_image(g->mlx, img->ptr);
			j++;
		}
		i++;
	}
	img = &g->assets.ammo_icon.img;
	if (img->ptr)
		mlx_destroy_image(g->mlx, img->ptr);
	i = ITEM_TYPES_NB;
	while (i--)
	{
		img = &g->assets.item_icons[i].img;
		if (img->ptr)
			mlx_destroy_image(g->mlx, img->ptr);
	}
}

#ifdef __linux__

void	free_all(t_game *g)
{
	if (!g)
		return ;
	if (g->mlx)
		mlx_loop_end(g->mlx);
	stop_audio(g);
	ft_lstclear(&g->allocated_pointers, &free);
	ft_destroy_textures(g);
	if (g->img.ptr)
		mlx_destroy_image(g->mlx, g->img.ptr);
	if (g->mlx_win && g->mlx)
	{
		mlx_destroy_window(g->mlx, g->mlx_win);
		mlx_destroy_display(g->mlx);
	}
	if (g->mlx)
		free(g->mlx);
	cleanup_unpacked_level(g);
}

#elif defined(__APPLE__)

void	free_all(t_game *g)
{
	if (!g)
		return ;
	stop_audio(g);
	ft_lstclear(&g->allocated_pointers, &free);
	ft_destroy_textures(g);
	if (g->img.ptr)
		mlx_destroy_image(g->mlx, g->img.ptr);
	if (g->mlx_win && g->mlx)
		mlx_destroy_window(g->mlx, g->mlx_win);
	if (g->mlx)
		free(g->mlx);
	cleanup_unpacked_level(g);
}

#else
# error "Unsupported operating system"
#endif
