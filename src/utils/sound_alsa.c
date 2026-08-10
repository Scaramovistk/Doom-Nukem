/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_alsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_ALSA

static snd_pcm_format_t	alsa_format(unsigned int bits)
{
	if (bits == 8)
		return (SND_PCM_FORMAT_U8);
	if (bits == 16)
		return (SND_PCM_FORMAT_S16_LE);
	if (bits == 24)
		return (SND_PCM_FORMAT_S24_3LE);
	return (SND_PCM_FORMAT_S32_LE);
}

static bool	open_alsa_channel(t_channel *channel)
{
	if (snd_pcm_open(&channel->pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
		return (false);
	if (snd_pcm_set_params(channel->pcm, alsa_format(channel->bits),
			SND_PCM_ACCESS_RW_INTERLEAVED, channel->channels, channel->rate, 1,
			100000) < 0)
		return (false);
	channel->device = 1;
	if (pthread_create(&channel->thread, NULL, audio_channel_thread, channel))
		return (false);
	channel->thread_started = true;
	return (true);
}

void	load_channel_wav(t_channel *channel, const char *path, bool loop)
{
	unsigned char	*file;
	size_t			size;

	close_channel(channel);
	if (pthread_mutex_init(&channel->mutex, NULL) != 0)
		return ;
	channel->mutex_initialized = true;
	file = NULL;
	if (!read_wav_file(path, &file, &size) || !parse_wav(channel, file, size))
		return (free(file), close_channel(channel));
	free(file);
	channel->loop = loop;
	if (!open_alsa_channel(channel))
		close_channel(channel);
}

void	audio_close_channel(t_channel *channel)
{
	snd_pcm_t	*pcm;

	if (!channel->mutex_initialized)
		return ;
	pthread_mutex_lock(&channel->mutex);
	channel->stop = true;
	pcm = channel->pcm;
	pthread_mutex_unlock(&channel->mutex);
	if (pcm)
		snd_pcm_drop(pcm);
	if (channel->thread_started)
		pthread_join(channel->thread, NULL);
	if (pcm)
		snd_pcm_close(pcm);
	free(channel->buf);
	pthread_mutex_destroy(&channel->mutex);
}

bool	audio_channel_ready(t_channel *channel)
{
	bool	ready;

	if (!channel->mutex_initialized)
		return (true);
	pthread_mutex_lock(&channel->mutex);
	ready = (!channel->device || channel->pos >= channel->len);
	pthread_mutex_unlock(&channel->mutex);
	return (ready);
}

#endif
