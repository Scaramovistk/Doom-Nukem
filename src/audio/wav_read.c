/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wav_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_ALSA

t_wav_uint	wav_u16(const unsigned char *data)
{
	return ((unsigned int)data[0] | ((unsigned int)data[1] << 8));
}

t_wav_uint	wav_u32(const unsigned char *data)
{
	return ((unsigned int)data[0] | ((unsigned int)data[1] << 8)
		| ((unsigned int)data[2] << 16) | ((unsigned int)data[3] << 24));
}

static bool	read_wav_data(int fd, unsigned char *file, size_t size)
{
	ssize_t	got;
	size_t	offset;

	offset = 0;
	while (offset < size)
	{
		got = read(fd, file + offset, size - offset);
		if (got <= 0)
			return (false);
		offset += (size_t)got;
	}
	return (true);
}

bool	read_wav_file(const char *path, unsigned char **file, size_t *size)
{
	struct stat	status;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	if (fstat(fd, &status) < 0 || status.st_size < 44)
		return (close(fd), false);
	*size = (size_t)status.st_size;
	*file = malloc(*size);
	if (!*file)
		return (close(fd), false);
	if (!read_wav_data(fd, *file, *size))
		return (close(fd), free(*file), *file = NULL, false);
	return (close(fd), true);
}

void	parse_wav_chunk(t_channel *channel, unsigned char *chunk,
		unsigned char **data, unsigned int *data_len)
{
	unsigned int	chunk_len;

	chunk_len = wav_u32(chunk + 4);
	if (!ft_memcmp(chunk, "fmt ", 4) && chunk_len >= 16)
	{
		channel->device = wav_u16(chunk + 8);
		channel->channels = wav_u16(chunk + 10);
		channel->rate = wav_u32(chunk + 12);
		channel->bits = wav_u16(chunk + 22);
	}
	else if (!ft_memcmp(chunk, "data", 4))
	{
		*data = chunk + 8;
		*data_len = chunk_len;
	}
}

#endif
