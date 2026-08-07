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
	const char *const	exts[3] = {".wav", ".ogg", ".mp3"};
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
# ifdef AUDIO_SDL2
	if (channel->device)
		SDL_CloseAudioDevice(channel->device);
	if (channel->buf)
		SDL_FreeWAV(channel->buf);
# else
	channel->stop = true;
	if (channel->pcm)
		snd_pcm_drop(channel->pcm);
	if (channel->thread_started)
		pthread_join(channel->thread, NULL);
	if (channel->pcm)
		snd_pcm_close(channel->pcm);
	free(channel->buf);
# endif
	ft_bzero(channel, sizeof(*channel));
}

# ifdef AUDIO_SDL2
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
	Uint8				*buffer;
	Uint32			length;

	buffer = NULL;
	length = 0;
	if (!SDL_LoadWAV(path, &want, &buffer, &length))
		return ;
	if (channel->device)
	{
		SDL_LockAudioDevice(channel->device);
		if (channel->buf)
			SDL_FreeWAV(channel->buf);
		channel->buf = buffer;
		channel->len = length;
		channel->pos = 0;
		channel->loop = loop;
		SDL_UnlockAudioDevice(channel->device);
		SDL_PauseAudioDevice(channel->device, 0);
		return ;
	}
	channel->pos = 0;
	channel->loop = loop;
	want.callback = audio_channel_callback;
	want.userdata = channel;
	channel->device = SDL_OpenAudioDevice(NULL, 0, &want, &spec, 0);
	if (!channel->device)
	{
		SDL_FreeWAV(buffer);
		return ;
	}
	channel->buf = buffer;
	channel->len = length;
	SDL_PauseAudioDevice(channel->device, 0);
}
# else

static unsigned int	wav_u16(const unsigned char *p)
{
	return ((unsigned int)p[0] | ((unsigned int)p[1] << 8));
}

static unsigned int	wav_u32(const unsigned char *p)
{
	return ((unsigned int)p[0] | ((unsigned int)p[1] << 8)
		| ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24));
}

static bool	read_wav_file(const char *path, unsigned char **file, size_t *size)
{
	struct stat	st;
	ssize_t		got;
	size_t		off;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0 || fstat(fd, &st) < 0 || st.st_size < 44)
	{
		if (fd >= 0)
			close(fd);
		return (false);
	}
	*size = (size_t)st.st_size;
	*file = malloc(*size);
	if (!*file)
		return (close(fd), false);
	off = 0;
	while (off < *size)
	{
		got = read(fd, *file + off, *size - off);
		if (got <= 0)
			return (close(fd), free(*file), *file = NULL, false);
		off += (size_t)got;
	}
	return (close(fd), true);
}

static bool	parse_wav(t_channel *channel, unsigned char *file, size_t size)
{
	unsigned char	*data;
	unsigned int	data_len;
	unsigned int	chunk_len;
	unsigned int	encoding;
	size_t			off;

	if (ft_memcmp(file, "RIFF", 4) || ft_memcmp(file + 8, "WAVE", 4))
		return (false);
	data = NULL;
	data_len = 0;
	encoding = 0;
	off = 12;
	while (off + 8 <= size)
	{
		chunk_len = wav_u32(file + off + 4);
		if ((size_t)chunk_len > size - off - 8)
			return (false);
		if (!ft_memcmp(file + off, "fmt ", 4) && chunk_len >= 16)
		{
			encoding = wav_u16(file + off + 8);
			channel->channels = wav_u16(file + off + 10);
			channel->rate = wav_u32(file + off + 12);
			channel->bits = wav_u16(file + off + 22);
		}
		else if (!ft_memcmp(file + off, "data", 4))
		{
			data = file + off + 8;
			data_len = chunk_len;
		}
		off += 8 + chunk_len + (chunk_len & 1U);
	}
	if (encoding != 1 || !data || !data_len || !channel->channels
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

static void	*audio_channel_thread(void *userdata)
{
	t_channel		*channel;
	snd_pcm_sframes_t	written;
	unsigned int	frames;

	channel = (t_channel *)userdata;
	while (!channel->stop)
	{
		if (channel->pos >= channel->len)
		{
			if (!channel->loop)
				break ;
			channel->pos = 0;
		}
		frames = (channel->len - channel->pos) / channel->frame_size;
		if (frames > 4096)
			frames = 4096;
		written = snd_pcm_writei(channel->pcm, channel->buf + channel->pos,
			frames);
		if (written < 0)
			written = snd_pcm_recover(channel->pcm, (int)written, 1);
		if (written < 0)
			break ;
		channel->pos += (unsigned int)written * channel->frame_size;
	}
	if (!channel->stop)
		snd_pcm_drain(channel->pcm);
	return (NULL);
}

void	load_channel_wav(t_channel *channel, const char *path, bool loop)
{
	unsigned char	*file;
	size_t			size;

	close_channel(channel);
	file = NULL;
	if (!read_wav_file(path, &file, &size)
		|| !parse_wav(channel, file, size))
		return (free(file), close_channel(channel));
	free(file);
	channel->loop = loop;
	if (snd_pcm_open(&channel->pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0
		|| snd_pcm_set_params(channel->pcm, alsa_format(channel->bits),
			SND_PCM_ACCESS_RW_INTERLEAVED, channel->channels, channel->rate,
			1, 100000) < 0)
		return (close_channel(channel));
	channel->device = 1;
	if (pthread_create(&channel->thread, NULL, audio_channel_thread, channel))
		return (close_channel(channel));
	channel->thread_started = true;
}
# endif

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
