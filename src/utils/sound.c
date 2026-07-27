/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	init_audio(t_game *g)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		g->audio.enabled = false;
		return ;
	}
	g->audio.enabled = true;
	if (!g->audio.sound_dir[0])
		ft_strlcpy(g->audio.sound_dir, SOUND_DIR, LINE_SIZE);
	sound_path(g->audio.music_path, sizeof(g->audio.music_path),
		g->audio.sound_dir, MUSIC_BASENAME);
}

void	play_sound_effect(t_game *g, const char *name)
{
	char			path[LINE_SIZE];
	t_channel		*channel;

	if (!g->audio.enabled)
		return ;
	if (!sound_path(path, sizeof(path), g->audio.sound_dir, name))
		return ;
	channel = pick_sfx_channel(g);
	load_channel_wav(channel, path, false);
}

void	start_background_music(t_game *g)
{
	if (!g->audio.enabled || !g->audio.music_path[0]
		|| g->audio.music.device)
		return ;
	load_channel_wav(&g->audio.music, g->audio.music_path, true);
}

void	stop_audio(t_game *g)
{
	int	i;

	if (!g)
		return ;
	close_channel(&g->audio.music);
	i = 0;
	while (i < SFX_CHANNELS_NB)
		close_channel(&g->audio.sfx[i++]);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void	update_audio(void)
{
}
