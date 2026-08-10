/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:50:21 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:50:35 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
