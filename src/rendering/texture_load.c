/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_load.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	load_world_textures(t_game *g)
{
	int	size;
	int	i;

	i = ft_check_amount();
	while (i--)
	{
		size = TEXTURE_SIZE;
		if (g->assets.textures[i].source)
			load_texture(&g->assets.textures[i], &size, g);
	}
	i = SPRITE_T - 1;
	while (++i <= DECAL_T)
	{
		size = TEXTURE_SIZE;
		if (g->assets.textures[i].source)
			load_texture(&g->assets.textures[i], &size, g);
	}
	i = -1;
	while (++i < SPRITE_FRAME_NB)
		if (g->assets.sprite_frames[i].source)
			load_texture(&g->assets.sprite_frames[i], &size, g);
}

static void	load_hud_textures(t_game *g)
{
	int	size;
	int	i;
	int	j;

	i = -1;
	while (++i < WEAPON_NB)
	{
		j = -1;
		while (++j < WEAPON_STATE_NB)
		{
			size = 32;
			if (g->assets.hud_weapons[i][j].source)
				load_texture(&g->assets.hud_weapons[i][j], &size, g);
		}
	}
	size = 32;
	if (g->assets.ammo_icon.source)
		load_texture(&g->assets.ammo_icon, &size, g);
}

static void	load_icon_textures(t_game *g)
{
	int	size;
	int	i;

	i = -1;
	while (++i < ITEM_TYPES_NB)
	{
		size = 32;
		if (g->assets.item_icons[i].source)
			load_texture(&g->assets.item_icons[i], &size, g);
	}
	i = -1;
	while (++i < ENEMY_TYPES_NB)
	{
		size = TEXTURE_SIZE;
		if (g->assets.enemy_icons[i].source)
			load_texture(&g->assets.enemy_icons[i], &size, g);
	}
}

static void	load_decoration_textures(t_game *g)
{
	t_texture	*t;
	int			size;
	int			i;

	i = -1;
	while (++i < DECORATION_TYPES_NB)
	{
		t = &g->assets.decoration_icons[i];
		if (t->source)
		{
			size = TEXTURE_SIZE;
			load_texture(t, &size, g);
			t->transparent_color = get_pixel(&t->img, 4 * (i == 5), 0);
			t->has_transparent_color = true;
		}
	}
	size = TEXTURE_SIZE;
	if (g->assets.vending_machine.source)
		load_texture(&g->assets.vending_machine, &size, g);
	size = TEXTURE_SIZE;
	if (g->assets.laptop.source)
		load_texture(&g->assets.laptop, &size, g);
}

void	load_all_textures(t_game *g)
{
	load_world_textures(g);
	load_hud_textures(g);
	load_icon_textures(g);
	load_decoration_textures(g);
}
