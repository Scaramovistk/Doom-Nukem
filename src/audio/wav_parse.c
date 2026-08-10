/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wav_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_ALSA

static bool	valid_wav(t_channel *channel, unsigned char *data,
		unsigned int data_len)
{
	if (channel->device != 1 || !data || !data_len || !channel->channels
		|| !channel->rate || (channel->bits != 8 && channel->bits != 16
			&& channel->bits != 24 && channel->bits != 32))
		return (false);
	channel->frame_size = channel->channels * (channel->bits / 8);
	channel->len = data_len - (data_len % channel->frame_size);
	channel->buf = malloc(channel->len);
	if (!channel->buf)
		return (false);
	ft_memcpy(channel->buf, data, channel->len);
	return (true);
}

bool	parse_wav(t_channel *channel, unsigned char *file, size_t size)
{
	unsigned char	*data;
	unsigned int	data_len;
	unsigned int	chunk_len;
	size_t			offset;

	if (ft_memcmp(file, "RIFF", 4) || ft_memcmp(file + 8, "WAVE", 4))
		return (false);
	data = NULL;
	data_len = 0;
	offset = 12;
	while (offset + 8 <= size)
	{
		chunk_len = wav_u32(file + offset + 4);
		if ((size_t)chunk_len > size - offset - 8)
			return (false);
		parse_wav_chunk(channel, file + offset, &data, &data_len);
		offset += 8 + chunk_len + (chunk_len & 1U);
	}
	return (valid_wav(channel, data, data_len));
}

#endif
