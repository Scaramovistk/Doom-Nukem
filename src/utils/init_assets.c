/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_assets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	init_world_textures(t_assets *assets)
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
	i = SPRITE_FRAME_NB;
	while (i--)
	{
		ft_int_image(&assets->sprite_frames[i].img);
		assets->sprite_frames[i].source = NULL;
	}
}

static void	init_weapon_assets(t_assets *assets)
{
	int	i;
	int	j;

	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
		{
			ft_int_image(&assets->hud_weapons[i][j].img);
			assets->hud_weapons[i][j++].source = NULL;
		}
		i++;
	}
	assets->hud_weapons[0][0].source = HUD_PISTOL_IDLE;
	assets->hud_weapons[0][1].source = HUD_PISTOL_FIRE;
	assets->hud_weapons[1][0].source = HUD_BLASTER_IDLE;
	assets->hud_weapons[1][1].source = HUD_BLASTER_FIRE;
	ft_int_image(&assets->ammo_icon.img);
	assets->ammo_icon.source = HUD_AMMO_ICON;
}

static void	init_item_enemy_icons(t_assets *assets)
{
	int	i;

	i = 0;
	while (i < ITEM_TYPES_NB)
	{
		ft_int_image(&assets->item_icons[i].img);
		assets->item_icons[i++].source = NULL;
	}
	assets->item_icons[0].source = HUD_ITEM0_ICON;
	assets->item_icons[1].source = HUD_ITEM1_ICON;
	assets->item_icons[2].source = HUD_ITEM2_ICON;
	assets->item_icons[3].source = HUD_ITEM3_ICON;
	i = 0;
	while (i < ENEMY_TYPES_NB)
	{
		ft_int_image(&assets->enemy_icons[i].img);
		assets->enemy_icons[i++].source = NULL;
	}
}

static void	init_decoration_assets(t_assets *assets)
{
	int	i;

	i = 0;
	while (i < DECORATION_TYPES_NB)
	{
		ft_int_image(&assets->decoration_icons[i].img);
		assets->decoration_icons[i].source = NULL;
		assets->decoration_icons[i++].has_transparent_color = false;
	}
	ft_int_image(&assets->vending_machine.img);
	assets->vending_machine.source = NULL;
	ft_int_image(&assets->laptop.img);
	assets->laptop.source = NULL;
}

void	ft_int_assets(t_assets *assets)
{
	ft_bzero(assets, sizeof(*assets));
	init_world_textures(assets);
	init_weapon_assets(assets);
	init_item_enemy_icons(assets);
	init_decoration_assets(assets);
	assets->has_sky = false;
	assets->has_sprite_frames = false;
	assets->ceiling_color = -1;
	assets->floor_color = -1;
}
