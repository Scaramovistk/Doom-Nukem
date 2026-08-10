/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	destroy_img(t_game *g, t_img *img)
{
	if (img->ptr)
		mlx_destroy_image(g->mlx, img->ptr);
}

static void	destroy_base_textures(t_game *g)
{
	int	i;

	i = TEXTURES_NB;
	while (i--)
		destroy_img(g, &g->assets.textures[i].img);
	destroy_img(g, &g->assets.floor_texture.img);
	destroy_img(g, &g->assets.ceiling_texture.img);
	destroy_img(g, &g->assets.sky_texture.img);
	i = SPRITE_FRAME_NB;
	while (i--)
		destroy_img(g, &g->assets.sprite_frames[i].img);
}

static void	destroy_weapon_textures(t_game *g)
{
	int	i;
	int	j;

	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
		{
			destroy_img(g, &g->assets.hud_weapons[i][j].img);
			j++;
		}
		i++;
	}
}

static void	destroy_icon_textures(t_game *g)
{
	int	i;

	destroy_img(g, &g->assets.ammo_icon.img);
	i = ITEM_TYPES_NB;
	while (i--)
		destroy_img(g, &g->assets.item_icons[i].img);
	i = ENEMY_TYPES_NB;
	while (i--)
		destroy_img(g, &g->assets.enemy_icons[i].img);
	i = DECORATION_TYPES_NB;
	while (i--)
		destroy_img(g, &g->assets.decoration_icons[i].img);
	destroy_img(g, &g->assets.vending_machine.img);
	destroy_img(g, &g->assets.laptop.img);
}

void	ft_destroy_textures(t_game *g)
{
	destroy_base_textures(g);
	destroy_weapon_textures(g);
	destroy_icon_textures(g);
}
