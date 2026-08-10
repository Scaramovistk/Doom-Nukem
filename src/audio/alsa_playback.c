/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alsa_playback.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_ALSA

static int	write_audio_chunk(t_channel *channel)
{
	snd_pcm_sframes_t	written;
	unsigned int		frames;
	unsigned int		position;

	frames = (channel->len - channel->pos) / channel->frame_size;
	if (frames > 4096)
		frames = 4096;
	position = channel->pos;
	pthread_mutex_unlock(&channel->mutex);
	written = snd_pcm_writei(channel->pcm, channel->buf + position, frames);
	if (written < 0)
		written = snd_pcm_recover(channel->pcm, (int)written, 1);
	pthread_mutex_lock(&channel->mutex);
	if (written < 0)
		return (-1);
	channel->pos += (unsigned int)written * channel->frame_size;
	return (0);
}

static bool	prepare_audio_chunk(t_channel *channel)
{
	if (channel->stop)
		return (false);
	if (channel->pos < channel->len)
		return (true);
	if (!channel->loop)
		return (false);
	channel->pos = 0;
	return (true);
}

void	*audio_channel_thread(void *data)
{
	t_channel	*channel;
	bool		stopped;

	channel = (t_channel *)data;
	pthread_mutex_lock(&channel->mutex);
	while (prepare_audio_chunk(channel))
	{
		if (write_audio_chunk(channel) < 0)
			break ;
	}
	stopped = channel->stop;
	if (!stopped)
		snd_pcm_drain(channel->pcm);
	pthread_mutex_unlock(&channel->mutex);
	return (NULL);
}

#endif
