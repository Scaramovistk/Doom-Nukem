/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:48:54 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
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

static void	init_menu_data(t_menu *menu)
{
	int	i;

	menu->level_count = 0;
	menu->selected = 0;
	menu->difficulty = 1;
	menu->active = false;
	i = 0;
	while (i < MENU_MAX_LEVELS)
		menu->levels[i++][0] = '\0';
}

static void	init_game_data(t_game *g)
{
	g->allocated_pointers = NULL;
	g->exit_status = EXIT_SUCCESS;
	g->campaign_mode = false;
	g->story_visible = false;
	g->story_is_debrief = false;
	g->campaign_level = 0;
	g->state = STATE_PLAYING;
	init_menu_data(&g->menu);
	ft_init_map(&g->map);
	ft_int_player(&g->player);
	ft_int_hud(&g->hud);
	ft_int_message(&g->message);
	ft_int_level_flow(&g->level);
	ft_int_audio(&g->audio);
	ft_int_projectiles(g->projectiles);
	ft_int_events(g->events);
}

static void	init_display_data(t_game *g)
{
	ft_int_assets(&g->assets);
	ft_int_image(&g->img);
	g->mlx = NULL;
	g->mlx_win = NULL;
	g->window_width = WIN_WIDTH;
	g->window_height = WIN_HEIGHT;
	g->render_x = 0;
	g->render_y = 0;
	g->fullscreen = false;
	g->unpacked_level = false;
	g->last_frame_time = 0.0;
	g->unpack_dir[0] = '\0';
	g->level_source[0] = '\0';
	g->delta_time = 0.0;
	g->hazard_damage_accumulator = 0.0;
}

void	init_game_struct(t_game *g)
{
	init_game_data(g);
	init_display_data(g);
}
