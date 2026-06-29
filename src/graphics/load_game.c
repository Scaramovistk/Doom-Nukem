/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:54:38 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:54:42 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	load_game(t_game *g)
{
	g->mlx = mlx_init();
	load_all_textures(g);
	ft_adjust_player_view(g);
	init_door_grid(g);
	open_window(g);
}

void	load_all_textures(t_game *g)
{
	int	size;
	int	i;

	size = TEXTURE_SIZE;
	i = ft_check_amount();
	while (i--)
		load_texture(&g->assets.textures[i], &size, g);
}

void	load_texture(t_texture *t, int *size, t_game *g)
{
	t->img.ptr = mlx_xpm_file_to_image(g->mlx, t->source, size, size);
	if (t->img.ptr == NULL)
		error("Unable to load texture", g);
	t->img.addr = mlx_get_data_addr(t->img.ptr, &t->img.bits_per_pixel,
			&t->img.line_length, &t->img.endian);
}

void	ft_adjust_player_view(t_game *g)
{
	if (g->player.look == 'N')
		g->player.orientation = 1.5 * M_PI;
	else if (g->player.look == 'S')
		g->player.orientation = M_PI / 2;
	else if (g->player.look == 'E')
		g->player.orientation = 0.0;
	else if (g->player.look == 'W')
		g->player.orientation = M_PI;
}

void	open_window(t_game *g)
{
	g->mlx_win = mlx_new_window(g->mlx, WIN_WIDTH, WIN_HEIGHT, "Cube 3D");
	move_mouse(g->mlx, g->mlx_win);
}
