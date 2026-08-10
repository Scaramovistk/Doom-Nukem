/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	show_message(t_game *g, const char *text, double duration)
{
	if (!text || duration <= 0.0)
		return ;
	ft_strlcpy(g->message.text, text, HUD_MESSAGE_LEN);
	g->message.timer = duration;
}

bool	update_message(t_game *g)
{
	if (g->message.timer <= 0.0)
		return (false);
	g->message.timer -= g->delta_time;
	if (g->message.timer < 0.0)
		g->message.timer = 0.0;
	return (true);
}

void	show_context_message(t_game *g)
{
	const char	*logs[5];

	logs[0] = "UAC LOG: PHOBOS EVACUATION FAILED";
	logs[1] = "LAB LOG: QUARANTINE BREACH CONFIRMED";
	logs[2] = "HELL RELAY: THE SIGNAL COMES FROM BELOW";
	logs[3] = "DEIMOS CORE: TELEPORTER LOCKED";
	logs[4] = "EARTH GATE: STABILITY FALLING";
	if (g->campaign_level >= 1 && g->campaign_level <= 5)
		show_message(g, logs[g->campaign_level - 1], MESSAGE_DISPLAY_TIME);
	else
		show_message(g, "TERMINAL OFFLINE", MESSAGE_DISPLAY_TIME);
}
