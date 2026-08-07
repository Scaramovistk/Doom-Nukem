/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level_loader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/27 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	reset_map_state(t_game *g)
{
	stop_audio(g);
	cleanup_unpacked_level(g);
	ft_destroy_textures(g);
	ft_lstclear(&g->allocated_pointers, &free);
	ft_init_map(&g->map);
	ft_int_assets(&g->assets);
	ft_int_player(&g->player);
	ft_int_events(g->events);
	ft_int_projectiles(g->projectiles);
	ft_int_audio(&g->audio);
	g->unpacked_level = false;
	g->unpack_dir[0] = '\0';
	g->level_source[0] = '\0';
	g->level.next_level[0] = '\0';
	g->hazard_damage_accumulator = 0.0;
}

static void	apply_difficulty(t_game *g)
{
	if (g->menu.difficulty == 0)
	{
		g->hud.health = 150;
		g->hud.max_health = 150;
		g->hud.inventory[ITEM_AMMO] += 20;
	}
	else if (g->menu.difficulty == 2)
	{
		g->hud.health = 75;
		g->hud.max_health = 75;
		if (g->hud.inventory[ITEM_AMMO] > 10)
			g->hud.inventory[ITEM_AMMO] -= 10;
	}
}

static int	campaign_level_from_path(char *path)
{
	char	*name;

	name = ft_strrchr(path, '/');
	if (name)
		name++;
	else
		name = path;
	if (ft_strlen(name) == 8 && name[0] == 'e' && name[1] == '1'
		&& name[2] == 'm' && name[3] >= '1' && name[3] <= '5'
		&& !ft_strcmp(name + 4, ".dnk"))
		return (name[3] - '0');
	return (0);
}

bool	load_level_path(t_game *g, char *path)
{
	char	*argv[2];
	bool	first_load;

	first_load = !g->level.started && !g->map.grid;
	if (!first_load)
		reset_map_state(g);
	ft_int_level_flow(&g->level);
	argv[0] = "doom-nukem";
	argv[1] = path;
	if (!ft_parse_file(2, argv, g))
		return (false);
	g->state = STATE_PLAYING;
	g->menu.active = false;
	g->campaign_level = campaign_level_from_path(path);
	if (!g->campaign_level)
		g->campaign_mode = false;
	g->story_visible = g->campaign_mode;
	g->story_is_debrief = false;
	compute_sector_origins(g);
	init_bsp_visibility(g);
	load_game(g);
	apply_difficulty(g);
	setup_hooks(g);
	start_level_flow(g);
	start_background_music(g);
	render(g);
	return (true);
}
