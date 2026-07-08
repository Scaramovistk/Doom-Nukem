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
	if (!g->mlx)
		g->mlx = mlx_init();
	load_all_textures(g);
	load_floor_ceiling_textures(g);
	ft_adjust_player_view(g);
	init_door_grid(g);
	init_audio(g);
	if (!g->mlx_win)
		open_window(g);
	else
	{
		hide_mouse(g->mlx, g->mlx_win);
		move_mouse(g->mlx, g->mlx_win);
	}
}

void	load_all_textures(t_game *g)
{
	int	size;
	int	i;
	int	j;

	i = ft_check_amount();
	while (i--)
	{
		size = TEXTURE_SIZE;
		if (g->assets.textures[i].source)
			load_texture(&g->assets.textures[i], &size, g);
	}
	size = TEXTURE_SIZE;
	if (g->assets.textures[SPRITE_T].source)
		load_texture(&g->assets.textures[SPRITE_T], &size, g);
	size = TEXTURE_SIZE;
	if (g->assets.textures[TRANSPARENT_T].source)
		load_texture(&g->assets.textures[TRANSPARENT_T], &size, g);
	size = TEXTURE_SIZE;
	if (g->assets.textures[DECAL_T].source)
		load_texture(&g->assets.textures[DECAL_T], &size, g);
	i = 0;
	while (i < SPRITE_FRAME_NB)
	{
		size = TEXTURE_SIZE;
		if (g->assets.sprite_frames[i].source)
			load_texture(&g->assets.sprite_frames[i], &size, g);
		i++;
	}
	i = 0;
	while (i < WEAPON_NB)
	{
		j = 0;
		while (j < WEAPON_STATE_NB)
		{
			size = 32;
			if (g->assets.hud_weapons[i][j].source)
				load_texture(&g->assets.hud_weapons[i][j], &size, g);
			j++;
		}
		i++;
	}
	size = 32;
	if (g->assets.ammo_icon.source)
		load_texture(&g->assets.ammo_icon, &size, g);
	i = 0;
	while (i < ITEM_TYPES_NB)
	{
		size = 32;
		if (g->assets.item_icons[i].source)
			load_texture(&g->assets.item_icons[i], &size, g);
		i++;
	}
}

void	load_texture(t_texture *t, int *size, t_game *g)
{
	t->img.ptr = mlx_xpm_file_to_image(g->mlx, t->source, size, size);
	if (t->img.ptr == NULL)
		error("Unable to load texture", g);
	t->img.addr = mlx_get_data_addr(t->img.ptr, &t->img.bits_per_pixel,
			&t->img.line_length, &t->img.endian);
}

static int	scale_color(int color, double scale)
{
	int	red;
	int	green;
	int	blue;

	red = fmin(255, ((color >> 16) & 0xFF) * scale);
	green = fmin(255, ((color >> 8) & 0xFF) * scale);
	blue = fmin(255, (color & 0xFF) * scale);
	return ((red << 16) | (green << 8) | blue);
}

static void	fill_surface_texture(t_texture *t, int color, t_game *g)
{
	int	x;
	int	y;
	int	pixel;

	t->img.ptr = mlx_new_image(g->mlx, TEXTURE_SIZE, TEXTURE_SIZE);
	if (t->img.ptr == NULL)
		error("Unable to create floor texture", g);
	t->img.addr = mlx_get_data_addr(t->img.ptr, &t->img.bits_per_pixel,
			&t->img.line_length, &t->img.endian);
	y = -1;
	while (++y < TEXTURE_SIZE)
	{
		x = -1;
		while (++x < TEXTURE_SIZE)
		{
			pixel = scale_color(color, 0.75 + 0.2 * (((x / 8) + (y / 8)) & 1));
			put_pixel(&t->img, x, y, pixel);
		}
	}
}

void	load_floor_ceiling_textures(t_game *g)
{
	int	size;

	size = TEXTURE_SIZE;
	if (g->assets.floor_texture.source)
		load_texture(&g->assets.floor_texture, &size, g);
	else
		fill_surface_texture(&g->assets.floor_texture, g->assets.floor_color, g);
	size = TEXTURE_SIZE;
	if (g->assets.ceiling_texture.source)
		load_texture(&g->assets.ceiling_texture, &size, g);
	else
		fill_surface_texture(&g->assets.ceiling_texture,
			g->assets.ceiling_color, g);
	size = TEXTURE_SIZE;
	if (g->assets.has_sky)
		load_texture(&g->assets.sky_texture, &size, g);
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
	g->mlx_win = mlx_new_window(g->mlx, WIN_WIDTH, WIN_HEIGHT, "Doom-Nukem");
	hide_mouse(g->mlx, g->mlx_win);
	move_mouse(g->mlx, g->mlx_win);
}
