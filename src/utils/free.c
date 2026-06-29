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

void	ft_destroy_textures(t_game *g)
{
	int		i;
	t_img	*img;

	i = TEXTURES_NB;
	while (i--)
	{
		img = &g->assets.textures[i].img;
		if (img->ptr)
			mlx_destroy_image(g->mlx, img->ptr);
	}
}

#ifdef __linux__

void	free_all(t_game *g)
{
	mlx_loop_end(g->mlx);
	if (!g)
		return ;
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
}

#elif defined(__APPLE__)

void	free_all(t_game *g)
{
	if (!g)
		return ;
	ft_lstclear(&g->allocated_pointers, &free);
	ft_destroy_textures(g);
	if (g->img.ptr)
		mlx_destroy_image(g->mlx, g->img.ptr);
	if (g->mlx_win && g->mlx)
		mlx_destroy_window(g->mlx, g->mlx_win);
	if (g->mlx)
		free(g->mlx);
}

#else
# error "Unsupported operating system"
#endif
