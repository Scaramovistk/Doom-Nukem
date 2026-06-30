/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:48:54 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:49:07 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_int_image(t_img *img)
{
	img->ptr = NULL;
	img->addr = NULL;
	img->bits_per_pixel = 0;
	img->line_length = 0;
	img->endian = 0;
}

void	ft_int_assets(t_assets *assets)
{
	int	i;

	i = TEXTURES_NB;
	while (i--)
	{
		ft_int_image(&assets->textures[i].img);
		assets->textures[i].source = NULL;
	}
	ft_int_image(&assets->floor_texture.img);
	ft_int_image(&assets->ceiling_texture.img);
	ft_int_image(&assets->sky_texture.img);
	assets->floor_texture.source = NULL;
	assets->ceiling_texture.source = NULL;
	assets->sky_texture.source = NULL;
	assets->has_sky = false;
	assets->ceiling_color = -1;
	assets->floor_color = -1;
}

void	ft_int_player(t_player *p)
{
	p->pos.x = 0;
	p->pos.y = 0;
	p->rotation_move = 0;
	p->key_rotation_move = 0;
	p->pitch = 0;
	p->pitch_move = 0;
	p->key_pitch_move = 0;
	p->mouse_move_pending = false;
	p->vertical_move = 0;
	p->lateral_move = 0;
	p->orientation = 0.0;
	p->look = ' ';
	p->mouse.x = WIN_WIDTH / 2;
	p->mouse.y = WIN_HEIGHT / 2;
}

void	ft_init_map(t_map *map)
{
	map->grid = NULL;
	map->sprites = NULL;
	map->sprite_count = 0;
	map->width = 0;
	map->height = 0;
}

void	init_game_struct(t_game *g)
{
	g->allocated_pointers = NULL;
	g->exit_status = EXIT_SUCCESS;
	ft_init_map(&g->map);
	ft_int_player(&g->player);
	ft_int_assets(&g->assets);
	ft_int_image(&g->img);
	g->mlx = NULL;
	g->mlx_win = NULL;
}
