/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_channel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_SDL2

static void	audio_callback(void *data, Uint8 *stream, int len)
{
	t_channel		*channel;
	unsigned int	remaining;
	unsigned int	copy_len;

	channel = (t_channel *)data;
	if (channel->pos >= channel->len && channel->loop)
		channel->pos = 0;
	if (channel->pos >= channel->len)
		return (ft_bzero(stream, len));
	remaining = channel->len - channel->pos;
	copy_len = (unsigned int)len;
	if (copy_len > remaining)
		copy_len = remaining;
	ft_memcpy(stream, channel->buf + channel->pos, copy_len);
	if (copy_len < (unsigned int)len)
		ft_bzero(stream + copy_len, len - copy_len);
	channel->pos += copy_len;
}

static bool	open_sdl_channel(t_channel *channel, SDL_AudioSpec *want)
{
	SDL_AudioSpec	spec;

	want->callback = audio_callback;
	want->userdata = channel;
	channel->device = SDL_OpenAudioDevice(NULL, 0, want, &spec, 0);
	return (channel->device != 0);
}

void	load_channel_wav(t_channel *channel, const char *path, bool loop)
{
	SDL_AudioSpec	want;
	Uint8			*buffer;
	Uint32			length;

	buffer = NULL;
	length = 0;
	if (!SDL_LoadWAV(path, &want, &buffer, &length))
		return ;
	close_channel(channel);
	channel->pos = 0;
	channel->loop = loop;
	if (!open_sdl_channel(channel, &want))
		return (SDL_FreeWAV(buffer));
	channel->buf = buffer;
	channel->len = length;
	SDL_PauseAudioDevice(channel->device, 0);
}

void	audio_close_channel(t_channel *channel)
{
	if (channel->device)
		SDL_CloseAudioDevice(channel->device);
	if (channel->buf)
		SDL_FreeWAV(channel->buf);
}

bool	audio_channel_ready(t_channel *channel)
{
	bool	ready;

	if (!channel->device)
		return (true);
	SDL_LockAudioDevice(channel->device);
	ready = (channel->pos >= channel->len);
	SDL_UnlockAudioDevice(channel->device);
	return (ready);
}

#endif
