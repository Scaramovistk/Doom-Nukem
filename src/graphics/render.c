/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:56:12 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:56:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	render(t_game *g)
{
	void	*mlx;
	void	*win;
	void	*img;

	update_time(g);
	ft_regenerate_image(g);
	draw_scene(g);
	if (MODE_BONUS)
		render_minimap(g);
	mlx = g->mlx;
	win = g->mlx_win;
	img = g->img.ptr;
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	return (EXIT_SUCCESS);
}

void	ft_regenerate_image(t_game *g)
{
	if (g->mlx && g->img.ptr)
		mlx_destroy_image(g->mlx, g->img.ptr);
	if (g->img.addr)
		g->img.addr = NULL;
	init_display_img(&g->img, g);
}

void	init_display_img(t_img *img, t_game *g)
{
	int	*bits;
	int	*line;
	int	*endian;

	img->ptr = mlx_new_image(g->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (img->ptr == NULL)
		error("Unable to create image", g);
	bits = &img->bits_per_pixel;
	line = &img->line_length;
	endian = &img->endian;
	img->addr = mlx_get_data_addr(img->ptr, bits, line, endian);
}

void	render_minimap(t_game *g)
{
	int	map[9];

	ft_values_setup(map);
	draw_minimap(g, map);
}
