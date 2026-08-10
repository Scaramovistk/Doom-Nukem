/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
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
	audio_close_channel(channel);
	ft_bzero(channel, sizeof(*channel));
}

t_channel	*pick_sfx_channel(t_game *g)
{
	int	i;

	i = 0;
	while (i < SFX_CHANNELS_NB)
	{
		if (audio_channel_ready(&g->audio.sfx[i]))
			return (&g->audio.sfx[i]);
		i++;
	}
	return (&g->audio.sfx[0]);
}
