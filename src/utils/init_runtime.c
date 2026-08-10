/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_runtime.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	init_channel(t_channel *channel)
{
	ft_bzero(channel, sizeof(*channel));
}

void	ft_int_audio(t_audio *audio)
{
	int	i;

	audio->enabled = true;
	init_channel(&audio->music);
	i = 0;
	while (i < SFX_CHANNELS_NB)
		init_channel(&audio->sfx[i++]);
	audio->music_path[0] = '\0';
	ft_strlcpy(audio->sound_dir, SOUND_DIR, LINE_SIZE);
}

void	ft_int_projectiles(t_projectile *projectiles)
{
	int	i;

	i = 0;
	while (i < PROJECTILE_MAX)
	{
		projectiles[i].pos = (t_position){0.0, 0.0};
		projectiles[i].velocity = (t_position){0.0, 0.0};
		projectiles[i].z = 0.0;
		projectiles[i].ttl = 0.0;
		projectiles[i].damage = 0;
		projectiles[i].size = PROJECTILE_SIZE;
		projectiles[i].color = YELLOW;
		projectiles[i].active = false;
		projectiles[i].from_enemy = false;
		i++;
	}
}

void	ft_int_events(t_world_event *events)
{
	int	i;

	i = 0;
	while (i < WORLD_EVENT_MAX)
	{
		events[i].action = EVENT_NONE;
		events[i].timer = 0.0;
		events[i].reload = 0.0;
		events[i].value = 0;
		events[i].target = 0;
		events[i].from_value = 0.0;
		events[i].to_value = 0.0;
		events[i].door_target = (t_coord){0, 0};
		events[i].repeat = false;
		events[i].active = false;
		events[i].message[0] = '\0';
		i++;
	}
}
