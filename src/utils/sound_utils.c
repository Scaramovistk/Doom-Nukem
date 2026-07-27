/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/23 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	sound_path(char *dst, size_t size, const char *dir, const char *name)
{
	static const char	*exts[3] = {".wav", ".ogg", ".mp3"};
	int					i;
	int					fd;

	i = 0;
	while (i < 3)
	{
		ft_strlcpy(dst, dir, size);
		ft_strlcat(dst, name, size);
		ft_strlcat(dst, exts[i], size);
		fd = open(dst, O_RDONLY);
		if (fd >= 0)
		{
			close(fd);
			return (true);
		}
		i++;
	}
	dst[0] = '\0';
	return (false);
}

void	close_channel(t_channel *channel)
{
	if (channel->device)
		SDL_CloseAudioDevice(channel->device);
	if (channel->buf)
		SDL_FreeWAV(channel->buf);
	channel->device = 0;
	channel->buf = NULL;
	channel->len = 0;
	channel->pos = 0;
	channel->loop = false;
}

static void	audio_channel_callback(void *userdata, Uint8 *stream, int len)
{
	t_channel		*channel;
	unsigned int	remaining;
	unsigned int	copy_len;

	channel = (t_channel *)userdata;
	if (channel->pos >= channel->len && channel->loop)
		channel->pos = 0;
	if (channel->pos >= channel->len)
	{
		ft_bzero(stream, len);
		return ;
	}
	remaining = channel->len - channel->pos;
	copy_len = (unsigned int)len;
	if (copy_len > remaining)
		copy_len = remaining;
	ft_memcpy(stream, channel->buf + channel->pos, copy_len);
	if (copy_len < (unsigned int)len)
		ft_bzero(stream + copy_len, len - copy_len);
	channel->pos += copy_len;
}

void	load_channel_wav(t_channel *channel, const char *path, bool loop)
{
	SDL_AudioSpec	want;
	SDL_AudioSpec	spec;

	close_channel(channel);
	if (!SDL_LoadWAV(path, &want, &channel->buf, &channel->len))
		return ;
	channel->pos = 0;
	channel->loop = loop;
	want.callback = audio_channel_callback;
	want.userdata = channel;
	channel->device = SDL_OpenAudioDevice(NULL, 0, &want, &spec, 0);
	if (!channel->device)
	{
		SDL_FreeWAV(channel->buf);
		channel->buf = NULL;
		return ;
	}
	SDL_PauseAudioDevice(channel->device, 0);
}

t_channel	*pick_sfx_channel(t_game *g)
{
	int				i;
	t_channel		*channel;

	i = 0;
	while (i < SFX_CHANNELS_NB)
	{
		channel = &g->audio.sfx[i];
		if (!channel->device || channel->pos >= channel->len)
			return (channel);
		i++;
	}
	return (&g->audio.sfx[0]);
}
