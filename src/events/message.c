/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
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
